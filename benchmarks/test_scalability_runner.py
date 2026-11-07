"""Offline parser checks; no compiled Ariadne dependencies."""
import unittest
import run_scalability_matrix as bench


class MatrixParseTests(unittest.TestCase):
    def test_exact_pair(self):
        output = '''
        | aligned-box    4 3 tree  iterate_leaves  0.6592 416 2031 32
        | aligned-box    4 3 robdd iterate_leaves  0.6561 416 108 32
        | aligned-box    4 3 tree  iterate_min     2.0259 416 2031 8
        | aligned-box    4 3 robdd iterate_min     2.0212 416 108 8
        | TREE_ITERATOR_COST shape=aligned-box d=4 f=3 leaves=416 nodes=2031 owner_begin_ms=0.0439 borrowed_begin_ms=0.0004 owner_visit_ms=0.6633 borrowed_visit_ms=0.6052
        '''
        rows = bench.parse_rows(output, 'aligned-box', 4, 3)
        self.assertEqual(len(rows), 4)
        result = bench.summarize('aligned-box', 4, 3, output, rows)
        self.assertEqual(result['status'], 'ok')
        self.assertEqual(result['borrowed_visit_ms'], '0.6052')
        self.assertEqual(result['tree_nodes'], '2031')
        self.assertAlmostEqual(float(result['robdd_over_tree_iterate_leaves']),
                               0.6561/0.6592)

    def test_missing_pair_is_not_silent(self):
        rows = bench.parse_rows('aligned-box 4 3 tree iterate_leaves 0.6 10 20 16',
                                'aligned-box', 4, 3)
        with self.assertRaisesRegex(ValueError, 'missing paired'):
            bench.summarize('aligned-box', 4, 3, '', rows)

    def test_complete_matrix(self):
        cases = list(bench.cases('all'))
        self.assertEqual(len(cases), 30)
        self.assertEqual(len(list(bench.cases('baseline'))), 25)
        self.assertEqual(len(list(bench.cases('expanded'))), 5)
        self.assertIn(('ball', 4, 3), cases)


if __name__ == '__main__':
    unittest.main()
