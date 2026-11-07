#!/usr/bin/env python3
"""Bounded, per-case collection of paving backend scaling results (not a test)."""

import argparse
import csv
import json
import os
from pathlib import Path
import platform
import re
import resource
import subprocess
import sys
import time

SHAPES = ('aligned-box', 'rotated-box', 'aligned-ellipsoid',
          'rotated-ellipsoid', 'ball')
OPERATIONS = ('iterate_leaves', 'iterate_min')
NUMBER = r'(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?'
ROW = re.compile(
    r'\b(?P<shape>aligned-box|rotated-box|aligned-ellipsoid|rotated-ellipsoid|ball)'
    r'\s+(?P<dimension>\d+)\s+(?P<fineness>\d+)'
    r'\s+(?P<backend>tree|robdd)\s+(?P<operation>[a-z][a-z_0-9]*)'
    r'\s+(?P<median_ms>' + NUMBER + r')'
    r'\s+(?P<leaves>\d+)\s+(?P<nodes>\d+)\s+(?P<iterations>\d+)\s*$'
)
DETAIL = re.compile(r'\bTREE_ITERATOR_COST\s+shape=(?P<shape>\S+)'
                    r'\s+d=(?P<dimension>\d+)\s+f=(?P<fineness>\d+)'
                    r'\s+leaves=(?P<leaves>\d+)\s+nodes=(?P<nodes>\d+)'
                    r'\s+owner_begin_ms=(?P<owner_begin_ms>' + NUMBER + r')'
                    r'\s+borrowed_begin_ms=(?P<borrowed_begin_ms>' + NUMBER + r')'
                    r'\s+owner_visit_ms=(?P<owner_visit_ms>' + NUMBER + r')'
                    r'\s+borrowed_visit_ms=(?P<borrowed_visit_ms>' + NUMBER + r')')
FIELDS = ('shape', 'dimension', 'fineness', 'status', 'reason',
          'tree_nodes', 'robdd_nodes', 'enabled_leaves',
          'tree_iterate_leaves_ms', 'robdd_iterate_leaves_ms',
          'tree_iterate_min_ms', 'robdd_iterate_min_ms',
          'robdd_over_tree_iterate_leaves', 'robdd_over_tree_iterate_min',
          'borrowed_visit_ms', 'owner_visit_ms', 'elapsed_seconds', 'log')
RAW_FIELDS = ('shape', 'dimension', 'fineness', 'backend', 'operation',
              'median_ms', 'leaves', 'nodes', 'iterations')


def parse_rows(output, shape, dimension, fineness):
    """Read only the operation table, rejecting mismatched scenario rows."""
    rows = []
    for line in output.splitlines():
        match = ROW.search(line)
        if not match:
            continue
        row = match.groupdict()
        if ((row['shape'], int(row['dimension']), int(row['fineness'])) !=
                (shape, dimension, fineness)):
            raise ValueError('unexpected scenario in benchmark output')
        rows.append(row)
    return rows


def summarize(shape, dimension, fineness, output, rows):
    summary = dict.fromkeys(FIELDS, '')
    summary.update(shape=shape, dimension=dimension, fineness=fineness,
                   status='ok')
    measurements = {(r['backend'], r['operation']): r for r in rows}
    for operation in OPERATIONS:
        tree = measurements.get(('tree', operation))
        robdd = measurements.get(('robdd', operation))
        if tree is None or robdd is None:
            raise ValueError('missing paired operation: ' + operation)
        if int(tree['leaves']) != int(robdd['leaves']):
            raise ValueError('tree/ROBDD enabled-leaf counts disagree')
        summary['tree_' + operation + '_ms'] = tree['median_ms']
        summary['robdd_' + operation + '_ms'] = robdd['median_ms']
        summary['robdd_over_tree_' + operation] = format(
            float(robdd['median_ms']) / float(tree['median_ms']), '.9g')
    tree = measurements[('tree', 'iterate_leaves')]
    robdd = measurements[('robdd', 'iterate_leaves')]
    summary['tree_nodes'] = tree['nodes']
    summary['robdd_nodes'] = robdd['nodes']
    summary['enabled_leaves'] = tree['leaves']
    details = DETAIL.search(output)
    if details:
        if ((details['shape'], int(details['dimension']), int(details['fineness'])) !=
                (shape, dimension, fineness)):
            raise ValueError('unexpected scenario in tree iterator diagnostic')
        summary['borrowed_visit_ms'] = details['borrowed_visit_ms']
        summary['owner_visit_ms'] = details['owner_visit_ms']
    else:
        raise ValueError('missing TREE_ITERATOR_COST diagnostic')
    return summary


def memory_limit(megabytes):
    if megabytes:
        limit = megabytes * 1024 * 1024
        resource.setrlimit(resource.RLIMIT_AS, (limit, limit))


def cases(phase):
    for d in (2, 3, 4):
        for f in (2, 3):
            if (d, f) == (4, 3) and phase == 'baseline':
                continue
            if (d, f) != (4, 3) and phase == 'expanded':
                continue
            for shape in SHAPES:
                yield (shape, d, f)


def write_csv(path, fields, rows):
    with path.open('w', newline='', encoding='utf-8') as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, extrasaction='ignore')
        writer.writeheader()
        writer.writerows(rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--binary', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--phase', choices=('baseline', 'expanded', 'all'),
                        default='all')
    parser.add_argument('--timeout-sec', type=int, default=180)
    parser.add_argument('--memory-mb', type=int, default=4096)
    args = parser.parse_args()
    if args.timeout_sec < 1 or args.memory_mb < 256:
        parser.error('timeout must be positive and memory >=256 MiB')
    binary = args.binary.resolve()
    if not binary.is_file():
        parser.error('benchmark executable missing: ' + str(binary))
    output_dir = args.output.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    results = []
    raw_rows = []
    metadata = {
        'git_sha': os.environ.get('GITHUB_SHA', 'unknown'),
        'machine': platform.platform(), 'processor': platform.processor(),
        'python_version': platform.python_version(), 'binary': str(binary),
        'phase': args.phase, 'timeout_seconds_per_case': args.timeout_sec,
        'memory_megabytes_per_case': args.memory_mb,
        'samples': 7, 'min_sample_ms': 12, 'max_iterations': 32768,
        'max_depth_bits': 28, 'no_retention': True, 'concurrency': 0,
    }
    cpuinfo = Path('/proc/cpuinfo')
    if cpuinfo.exists():
        for line in cpuinfo.read_text(errors='replace').splitlines():
            if line.startswith('model name'):
                metadata['cpu_model'] = line.partition(':')[2].strip()
                break
    (output_dir / 'metadata.json').write_text(
        json.dumps(metadata, indent=2) + '\n', encoding='utf-8')

    for shape, dimension, fineness in cases(args.phase):
        logname = f'{shape}_d{dimension}_f{fineness}.log'
        command = [str(binary), '--dimensions', str(dimension),
                   '--finenesses', str(fineness), '--shapes', shape,
                   '--max-depth-bits', '28', '--samples', '7',
                   '--min-sample-ms', '12', '--max-iterations', '32768',
                   '--tree-iterator-cost', '--no-retention', '--concurrency', '0']
        print(f'Running {shape} d={dimension} f={fineness}', flush=True)
        began = time.monotonic()
        status, reason, log = 'ok', '', ''
        rows = []
        try:
            completed = subprocess.run(command, capture_output=True,
                                       text=True, errors='replace',
                                       timeout=args.timeout_sec,
                                       preexec_fn=lambda: memory_limit(args.memory_mb),
                                       check=False)
            log = completed.stdout + '\n' + completed.stderr
            if completed.returncode != 0:
                status, reason = 'failed', 'exit code ' + str(completed.returncode)
            else:
                rows = parse_rows(log, shape, dimension, fineness)
        except subprocess.TimeoutExpired as exc:
            status, reason = 'timeout', f'exceeded {args.timeout_sec}s'
            chunks = (exc.stdout or b'', exc.stderr or b'')
            log = '\n'.join(x.decode('utf-8', 'replace') if isinstance(x, bytes) else x
                            for x in chunks)
        except (OSError, ValueError) as exc:
            status, reason = 'failed', str(exc)
        (output_dir / logname).write_text(log, encoding='utf-8')
        try:
            if status == 'ok':
                summary = summarize(shape, dimension, fineness, log, rows)
            else:
                summary = dict.fromkeys(FIELDS, '')
                summary.update(shape=shape, dimension=dimension, fineness=fineness)
        except ValueError as exc:
            summary = dict.fromkeys(FIELDS, '')
            summary.update(shape=shape, dimension=dimension, fineness=fineness)
            status, reason = 'incomplete', str(exc)
        summary.update(status=status, reason=reason,
                       elapsed_seconds=f'{time.monotonic()-began:.3f}', log=logname)
        results.append(summary)
        raw_rows.extend(rows)
        print(f'  {status}: {reason or "paired iteration measurements collected"}',
              flush=True)
        write_csv(output_dir / 'summary.csv', FIELDS, results)
        write_csv(output_dir / 'operations.csv', RAW_FIELDS, raw_rows)

    markdown = [
        '# Paving backend scaling matrix',
        '',
        f"Commit: {metadata['git_sha']}; phase: {args.phase}; "
        f"per-case cap: {args.timeout_sec}s / {args.memory_mb} MiB.",
        '',
        '| Shape | D | F | Status | Leaves | Tree nodes | ROBDD nodes | '
        'Tree iterate (ms) | ROBDD iterate (ms) | ROBDD/tree | Note |',
        '|---|---:|---:|---|---:|---:|---:|---:|---:|---:|---|',
    ]
    for r in results:
        markdown.append('| ' + ' | '.join(str(r[k]) for k in (
            'shape', 'dimension', 'fineness', 'status', 'enabled_leaves',
            'tree_nodes', 'robdd_nodes', 'tree_iterate_leaves_ms',
            'robdd_iterate_leaves_ms', 'robdd_over_tree_iterate_leaves',
            'reason')) + ' |')
    markdown += ['', 'Times from independent seven-sample within-case medians. '
                 'Cross-machine and cross-commit comparison requires matching '
                 'hardware and build settings. Timeouts and failures are not '
                 'silently excluded from the matrix.', '']
    report = '\n'.join(markdown)
    (output_dir / 'summary.md').write_text(report, encoding='utf-8')
    print(report)
    if os.environ.get('GITHUB_STEP_SUMMARY'):
        with open(os.environ['GITHUB_STEP_SUMMARY'], 'a', encoding='utf-8') as stream:
            stream.write(report + '\n')
    return 0  # Measurements are diagnostics, not pass/fail performance tests.


if __name__ == '__main__':
    sys.exit(main())
