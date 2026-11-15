// Generated from real SPRE22 evolver enclosures. Do not edit.
// Horizon: 1/16; max_jumps: 1; enclosures: 48
#ifndef ARIADNE_SPRE22_REAL_ENCLOSURES_GENERATED_HPP
#define ARIADNE_SPRE22_REAL_ENCLOSURES_GENERATED_HPP
#include <bit>
#include <cstdint>
#include <utility>
#include <vector>

#include "function/constraint.hpp"
#include "function/taylor_function.hpp"
#include "set/function_set.hpp"

namespace Ariadne::SPRE22Fixture {
inline std::vector<ValidatedConstrainedImageSet> real_enclosures() {
    std::vector<ValidatedConstrainedImageSet> sets;
    sets.reserve(48u);
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511a7bffULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511a7bffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511a7bffULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c60076a6f34f740ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e85a3637821728aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84af8afcdULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4efc363ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefa0b551f9be422ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903349777ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89e83a2cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9b0ae9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63f8628ba973dfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c246066c0c8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb883ca8aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f55629d3ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b646489da34ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fede74eea1d24ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a2223e3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf0301afeULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb5c52f655309ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd542b73150cbbULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e9352d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358cd5d4fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7ecfbebffdbc0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf396373472314f8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c21280b64eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef743086286ce47ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f52210918519521ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fa46b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee0259cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c913660ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171e4c57a21ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9d29481e580a0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad0bc3fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8daaa00cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe404ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2b8fef2b821eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebba350676c5b86ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9f1dc49a04cebdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc4fdb30ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b78401824bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12b9efd86def85ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb78cb5bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e27aa70ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af7262447336ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee285064fbeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7cf487d2e5f0d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c68de7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7abd3bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccfc1308ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce1ec63ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fddbdfe00766a0dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a47914ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018ee0cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d62d3b91ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c1126e45bbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007c65fb6137a4aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3cc5a4f12522dULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8bdc447e446484ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d18044e7017ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c5d2d56ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0157678958b420ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d26c01ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235ad20449ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e70150e742ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d194b03e899ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a920fbd949e91ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784b2968ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fcab61ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71544469382ffdULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184cd7ba03fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fcb001fa16a4a42ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eeeb52ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a344f44ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e3ed683ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee736401efab9c0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005b6b7f149d7a3ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f89c2cb51459ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511a7bffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511a7bffULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f34f740ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b0310ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b030eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b030fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f351093ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e85a43be8f32015ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84af9edafULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f1dafcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefa0c0f16cb3c9eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903357e8dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89e97406ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9c03cfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63f8a85a789c51ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c246067b370ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb8846953ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f5571328ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648ad961ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fedf94fc8d53bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a231a67ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf030bec3ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb5c52f5322d4ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd543e33157addULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e949109ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358cfa985ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6f99dcc382f06ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf39637347244f27ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c2127fd795ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef743086287ee4aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5166de9f4d3ebbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fae57bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee03518ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c925d49ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171ca64f514ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9d11a6c9acdd91ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad16002ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8daafeefULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe404ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07963901eabc53bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb972c5e273e2eULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9f1f690ec43facULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc51a4c4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b784031723ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12bab9e4241d52ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7a21b3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e288c68ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af726245ae82ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee28507d570ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7cf55575e55b23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c73505ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7b3065ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccfcac58ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce2a298ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fddbe65fe6d3613ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a52b29ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018ea56ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d62ddef9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c1126f0221ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007c66d1e3b33d6ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3cc67eb5b00bdULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8ac4913a96f382ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d1804503a25ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c5ed497ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f00a82d5505d7dcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d34e39ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235afafcbbULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e70151b85dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d193872114fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6983c714b2726fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784b9cb1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fac363ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf715444693899bbULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184a760c4bbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc9eadba0c43614ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee79bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a33b629ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e3f4394ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363ff849c98eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005a51f6ec3563eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f89c23ae56b7ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b030fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b030eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f351093ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b0311ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b0311ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b0311ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c60076a6f351094ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e854799646a7e93ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84af9edb8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f1c08dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9a23329a7cc9cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903357e93ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89e9740cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9c0473ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63a7b05ffa21a7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c246067b375ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb8846956ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f557132dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648ad9b4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fd96b32f252eeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a231a6bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf030bec5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c51c543fc1348ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd3a2db3153121ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e94b6dbULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358ad5b63ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7ebbaa562c4e6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf39637347244d68ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c212819ce6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef74308626e5aa7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f52209039cbff6dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fae585ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee117c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c7862baULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171e4c5ff74ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9d2938887ed04fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad16006ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dab960cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe40bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2b8fef4affb0ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebba2f4ec1d2f71ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9e99990c63467cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc51a4f8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b78402f0e5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f126e3da895d42eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7a20baULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e288c6dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af726237ca02ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee28507b9adULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7c7f7fe4bdf0f1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c73510ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7b3068ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccfd91d1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce2a382ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdd45cfe78e7823ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a52b2dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018ea56ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d62e52e9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c1126f0298ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007b71ed692b141ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3b961654a5509ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8bd938bd551d01ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d180450a820ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c467a0fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f01560a74ca4d86ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d375e1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235ad35986ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7013f2526ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d194b04c7d1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a90aba2e72005ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784b9b6dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fd5226ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71544469258becULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184cd7bc1d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fcaff6b96077321ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee7a1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a3500f8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e2beab4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee736401efae27fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005b6ac562cde82ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f85cf93d1cfeULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b0311ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b0311ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f351094ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b8a22ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a22ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a22ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c60076a6f3529e7ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e854871d53c22e8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afb2ba0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f3d82cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9a2ed20d71d16ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff29033665b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89eaade9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9cfd5bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63a7f62ec9463aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c246068a622ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb8850821ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f557fc85ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648bd8e4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fd97d33d10a07ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a2410f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf031628bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c51c543e9e312ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd3a4053155602ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e95f519ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358afa79dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6f85c859c11faULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3963734725879aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c21280be33ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef74308626f7aaeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f516665c0c86cb6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fb844dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee12743ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c7989a6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171ca657c77ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9d119733d3bcaaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad203cbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dabf4f3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe40aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07963901ecb42ccULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb96d0e2d7d92eULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9e9b3d8122a9b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc536e92ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b7840485c3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f126f07b44bfbf0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7b7718ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e296e6cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af7262390556ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee285093f66ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7c804d87bd5539ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c7dc2fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7ba393ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccfe2b24ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce359b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdd4637e5854154ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a5dd42ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018e6a0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d62ef654ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c1126fbefeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007b72c3eba6a6fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3b96f01825d63ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8ac18579a8bc42ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d1804527234ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c482155ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f00a6d04078223eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d4581bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235afc51f9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7013ff646ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d193872f1fdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf698262fa061015ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784c0eb8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fb6a29ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7154446925f5abULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184a760e944ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc9ea2795628933ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee3eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a3467deULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e2c57c5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363ff849f550ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005a513d3a67009ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f85cf035fa2bULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a22ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b8a22ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f3529e7ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b8a1fULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a1dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a1eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c675f389446e288ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e85a51459c4cdccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afb2b93ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f3f293ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefa0cc90dfa9532ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff29033665a6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89eaade1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9cfcb2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63f8ee2947c4c8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c246068a619ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb885081cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f557fc7cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648bd88cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fee0b50a78d50ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a2410eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf0316287ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb5c52f40f29eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd5450d315dfafULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e95cf43ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358d1f5b7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6063fac705eb2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf39637347258954ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c2127ef8deULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7430862890e4eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f50acb42648d655ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fb8442ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee04495ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c938431ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171b0047008ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9cfa057501aec9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad203c5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dab5dd2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe404ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789b904e2c0857ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb8b0844e21369ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9f210d8383b0ebULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc536e57ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b78404abfdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12bb83efda4b3eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7b780bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e296e63ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af726246e9d1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee285095b23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7cf62318e4c584ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c7dc22ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7ba38fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccfd45a9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce358ccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fddbecdfc64021dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a5dd3bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018e6a0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d62e8262ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c1126fbe85ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007c67a8662ed62ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3cc7587939841ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe89acddf6e94e3bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d1804520434ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c607bd5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efff1e64165c3ffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d43071ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235b23f52bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e701528977ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d1925e03a04ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf68757e6bd02c28ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784c0ffbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502f8db63ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7154446939037aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee681848145e937ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc8d597a01e14b4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee3e4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a331d0cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e3fb0a5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363fd198d95aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40059386ec3cd400ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f89c1aaf7bc2ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a1eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b8a1dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c675f389446e288ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c112fULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c112fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c112fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c60076a6f354339ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e85a5ecca967ba3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afc697fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f60a35ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefa0d830529edf0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903374cc2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ebe7bfULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9df59fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63f933f816ed4cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606998c4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb885a6e7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f558e5d6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648cd7beULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fee1d5186456cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a250770ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf032064dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb5c52f2ec269ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd546373157b5dULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e970d85ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358d441efULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee512e18ca86ac2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3963734726c387ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c2127e1a2eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef74308628a2e57ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4fe5135a88b7e5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fc230aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee05416ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c94ab1dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe80117195a3eaffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9ce26420567db8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad2a789ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dabbcb8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe404ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d3907dac4b74ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb7ee42b9c2d12ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9f22b1f8432268ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc5537f4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b7840640dfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12bc4dfb90794fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7cce67ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2a5063ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af7262482525ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850ae0d9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7cf6f0bbe43001ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c88342ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7c16bdULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccfddefcULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce40f03ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fddbf35fa5ace2cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a68f50ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018e2eaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d62f25ccULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c112707aebULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007c687ee8aa6efULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3cc8323ba6b3dULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe88952ab33b74caULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d180453ce4aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c62231aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe9371d8bfa4f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d512abULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235b4ceda3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e701535a98ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d19134e62bfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf676735c2edcbbbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784c8348ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502f6f367ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71544469396d3aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee681845b2b0db4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc7c0539f77e61cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee02dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a3283f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e401db5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363faae7e927ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400581ee69b650e8ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f89c11b7abdfULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c112fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c112fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f354339ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c1131ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1131ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1131ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c60076a6f35433aULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e85494a460dc771ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afc6984ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f5efc5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9a3a718066da7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903374cc6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ebe7c4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9df641ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63a83bfd986ad1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606998c8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb885a6e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f558e5d9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648cd811ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fd98f34afc120ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a250774ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf032064fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c51c543d7b2ddULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd3a52f3157ae3ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e973352ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358b1f3d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee604fe65d53b6bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3963734726c1c8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c2127fdf7eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7430862709ab2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f50ac3b47c4c7faULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fc2313ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee136c0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c7ab08eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171b004f977ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9cf9f5df28a6b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad2a78dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dac53d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe40aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789b904e4b85e8ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb8aacc99280a6ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9e9ce1f5e20d52ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc553826ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b784061a9dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f126fd1c00223d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7ccd72ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2a5067ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af72623a40a2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850ac516ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7c811b2abcb98dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c8834eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7c16bfULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccfec474ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce40fecULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdd469fe37c0a86ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a68f56ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018e2eaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d62f99bcULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c112707b63ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007b739a6e2239eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3b97c9dba65beULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe89a9d235fc2740ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d1804543c42ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c49c894ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3effef2c184bba9eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d53a51ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235b254a68ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e70140c761ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d1925e11c24ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf68741a5124e5faULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784c8202ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502f9822bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71544469265f6aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184814610b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc8d4e394bd920eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee032ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a33cec2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e2cc4d6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363fd199081eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005937b512000b6ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f85ce736d757ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1131ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c1131ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f35433aULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c9841ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9841ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9841ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c60076a6f355c8dULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e854a22b6df6c09ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afda76aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f8075fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9a4610f35be54ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff29033833e1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ed219eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9eef29ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63a881cc678f70ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606a8b73ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb88645b3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f559cf2fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648dd740ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fd9a1358e783bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a25fdf9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf032aa14ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e26fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c51c543c582a7ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd3a65b3159fc3ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e98718eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358b44003ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee511a0460e4140ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3963734727fbf7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c2127f00c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef743086271bab3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4fe4219d822279ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fcc1daULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee1463cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c7bd774ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe80117195a47679ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9ce2548a7d8e77ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad34b51ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dacb2b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe40aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d3907dcbc904ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb7e88b04d25d9ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9e9e866aa17118ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc5701bcULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b78407af79ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12709bcbb84bccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7e23cbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2b3261ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af72623b7bf2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850c4acdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7c81e8cdbc1df6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c92a6cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7c89e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccff5dc4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce4c620ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdd4707e172d3beULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a7416aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018df34ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d6303d24ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c1127137c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007b7470f09dccdULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3b98a39f26e19ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe88921ef24f5e04ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d1804560654ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c4b6fd5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe90b7afa6fd76ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d61c8aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235b4e42d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e70141987fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d19134f464bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6765d1a843a1b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784cf54cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502f79a2dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7154446926c927ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee681845b2b3827ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc7bf9f94188db1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eedc7bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a3335a7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e2d31e4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363faae81aecULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400581e2ce999089ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f85cde3fb484ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9841ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c9841ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f355c8dULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b8a23ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a21ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a22ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f3529e7ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e84ebcf50b37926ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afb2b9dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f3bdb2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9391133b3a5c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff29033665aeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89eaade7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9cfdf8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f6356fe344ac7aaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c246068a621ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb8850820ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f557fc84ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648bd933ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fc4ef16fa86b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a2410f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf031628bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bedc55892d387ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd202ff3159ec7ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e961addULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e03588d5971ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7ea795ec5adddULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf396373472585d1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c21282837bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef743086255e704ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5220175b4669acULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fb8453ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee209e7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c5f8f11ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171e4c684c5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9d2928f2a5968eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad203cdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dac8c0bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe411ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2b8fef6a7d41ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebba29980ce488bULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9e156d7ec1a519ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc536ebaULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b784045f79ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12228b78bdadcaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7b7617ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e296e67ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af72622b20ccULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee28509239cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7c0a77f695e5d1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c7dc36ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7ba393ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccff109aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce35a9cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdccda1cea68093ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a5dd45ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018e6a0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d62f6a42ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c1126fbf72ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007a7ddf711e77eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3a6687ba2fe1aULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8bd62cfc65d4d9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d180452e024ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c2fc6c5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0154ad603be6b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d47fbbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235ad4aebfULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7012d6309ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d194b05a705ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a8f478839a157ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784c0d70ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fdf8eaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7154446912e7d9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184cd7be360ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fcafeb78aa49bfbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee3efULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a35b2acULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e18fee4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee736401efb0b3eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005b6a0bb0fe560ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f81dc5bd8ad5ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a22ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b8a21ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f3529e7ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c1134ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1132ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1133ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f35433aULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e84eca7c1851444ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afc6985ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f5d551ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef939cb2ae2ee36ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903374cc8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ebe7c7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9df6e2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f6357440319e85cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606998cbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb885a6ebULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f558e5dcULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648cd863ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fc50117d93cd4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a250778ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf0320652ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bedc55880a351ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd20429315c3a8ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e97591bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e03588fa5abULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6f71b3efff4b7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3963734726c004ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c21281a4c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef743086257070aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5165ece2439a9fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fc231cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee21966ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c60b5fdULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171ca6603d4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9d11879dfa9bbeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad2a793ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8daceaf1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe411ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07963901eeac05dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb96756788fe92ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9e1711f380faa3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc553854ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b78405f458ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1223558473cf7fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7ccc77ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2a5066ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af72622c5c20ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850aa952ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7c0b4599954408ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c88357ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7c16bfULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97ccffa9e9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce410d2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdcce09cc9d46edULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a68f5bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018e2eaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d6300daaULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c112707bd8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007a7eb5f39a04eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3a67617e30675ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8abe79b8ba845bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d180454aa38ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c316e09ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f00a5732bea6c5eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d561f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235afda72dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7012e3428ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d193873d2a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6980fedf59ad94ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784c80bbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fc10edULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71544469135199ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184a7610dc8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc9e9738a00dc48ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee038ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a351992ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e196bf6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363ff84a210fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005a508388989d3ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f81dbcb66801ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1133ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c1132ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f35433aULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c1136ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1134ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1135ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f35433aULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8490053cfc6257ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afc6989ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f5badfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef8cfef3dbf6f85ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903374cccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ebe7ccULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9df785ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63064c089b65d5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606998cfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb885a6ecULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f558e5dfULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648cd8b7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fb072fb02b884ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a25077cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf0320653ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e270ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b89c56d2993c6ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd06321315c32dULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e977ee9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e03586d578bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7e938182896b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3963734726be43ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c212836a16ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef74308623d7366ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f521f9e7cc0d3e7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fc2325ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee2fc0fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c46bb6fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171e4c70a19ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9d29195ccc5cd1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad2a797ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dad820eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe417ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2b8fef89fad3ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebba23e057f1c6cULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9d9141f11feaaaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc553885ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b78405ce15ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11d6d948e57c4fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7ccb7dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2a5069ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af72621e779dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850a8d8fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7b9570086dcf52ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c88361ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7c16c1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97cd008f63ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce411b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdc5573b5be8337ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a68f60ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018e2e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d630819bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c112707c4eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007989d17911cfcULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3936f91d300f6ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8bd3213b768c1fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d1804551830ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c191382ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0153504bad7fb4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d5899bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235ad603fdULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7011ba0f0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d194b068640ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a8de36d8c2291ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784c7f76ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fe9fb0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf715444690043cbULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184cd7c04f0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fcafe037f41c4d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eee03dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a366461ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e061318ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee736401efb33fdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005b6951ff2ec3fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f7de923d937aULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1135ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c1134ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f35433bULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c9846ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9846ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9846ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c615f3894472612ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8490ddadcdf44eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afda775ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f7d280ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef8d0a934eeaff5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff29033833eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ed21a9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9ef071ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f630691d76a82a9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606a8b7cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb88645b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f559cf37ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648dd7e8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fb084fbe16da2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a25fe02ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf032aa19ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e26fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b89c56d176390ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd0644d315de45ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e98bd2aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e03586fa3c9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6f5d9f863d755ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3963734727f876ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c212828b63ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef74308623e9371ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51657403bec888ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fcc1edULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee30b8fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c47e25dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171ca668b37ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9d117808217ad5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad34b5bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dade0f3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe417ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07963901f0a3defULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb9619ec39d3c5ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9d92e665df3298ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc570222ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b7840762f7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11d7a3549b97fcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7e21d9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2b3269ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af72621fb2f2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850c1347ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7b963dab6d277bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c92a82ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7c89edULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97cd0128b6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce4c7f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdc55dbb3b546bcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a74176ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018df33ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d6312505ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c1127138b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400798aa7fb8d56eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3937d2e12ec74ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8abb6df7cc4becULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d180456e248ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c1abaccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f00a416175cb655ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d66bd6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235afefc6cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7011c7213ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d193874b350ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf697f9ac4ad4afbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784cf2c1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fcb7b5ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7154446900ad8cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68184a7613250ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc9e8bf7e9f2f5cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eedc86ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a35cb48ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e06802aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363ff84a4cd0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005a4fc9d6ca39dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f7de893e5591ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9846ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c9846ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c655f3894472612ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c9844ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9844ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9844ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c60076a6f355c8dULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e84ed803256af99ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afda771ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f7ecf1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef93a85221236ccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff29033833e6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ed21a5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9eefceULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f635789d1e90917ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606a8b78ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb88645b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f559cf35ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648dd795ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fc51318b7f2f1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a25fdfeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf032aa17ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e26fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bedc5586e731cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd205533159f49ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e98975dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e035891f1e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee603bd1f3a17f7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3963734727fa37ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c21280c618ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7430862582714ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f50abc26940b992ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fcc1e5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee228e8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c61dceaULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171b00582e5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9cf9e6494f9ea2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad34b57ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dad49d8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe410ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789b904e6b0379ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb8a514e436d20ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9e18b66840508eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc5701f2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b784078939ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12241f9029f156ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7e22d4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2b3266ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af72622d9771ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850c2f0bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7c0c133c94a255ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c92a78ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7c89ebULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97cd00433fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce4c70aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdcce71ca940d4cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a74171ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018df33ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d630b116ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c11271383fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007a7f8c761591fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3a683b40a689aULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe89a6c6750effa8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d1804567450ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c331551ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3effec71ef31b0d1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d64430ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235b269f9fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7012f0549ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d1925e1fe3fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6872b636799fabULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784cf408ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502fa28f3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7154446913bb5aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6818481463833ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc8d42f895d0f61ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eedc80ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a348079ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e19d908ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363fd19936e1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4005936fb6032d6cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f81db3b6dffdULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9844ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c9844ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f355c8dULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511d1f53ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f51ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f52ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f3575e0ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e84ee58a3284aefULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afee54fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4fa0483ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef93b3f19417f69ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903391af9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ee5b7cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9fe8b1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f6357cfa0b829cfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606b7e1dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb886e47cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f55ab886ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648ed6bfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fc5251996a90aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a26f47fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf0334dd9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e26fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bedc5585c42e6ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd2067d3160d69ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e99d591ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358943e11ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5105eff74178aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf39637347293463ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c2127fe75cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7430862594710ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4fe32fe07b8cf6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fd60a8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee23861ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c6303cdULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe80117195a501efULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9ce244f4a49f32ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad3ef18ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dada8b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe410ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d3907deb4695ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb7e2d44fe61d0ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9e1a5adcffa696ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc58cb7eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b784091e0dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1224e99be0133eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7f7927ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2c145eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af72622ed2bbULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850db4b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7c0ce0df9400abULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c9d194ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7cfd14ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97cd00dc8cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce57d3dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdcced9c88ad3aaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a7f383ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018db7dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d631547cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c11271f4a3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4007a8062f8911f0ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3a691504b172bULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe888f13316346a0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d1804583e58ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c34bc89ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe8dfd868e558aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d72665ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235b4f9804ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7012fd65fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d19135029d7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf67646d8d997793ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784d674eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502f840f3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71544469142514ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee681845b2b6298ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc7beeb88b9353fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eed8caULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a33e75cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e1a4614ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363faae84cafULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400581d7337cd02aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f81daac02254ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f52ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511d1f51ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f3575e0ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511d1f56ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f58ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f57ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f3575e1ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8491b61e9f8664ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84afee562ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4f9ea27ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef8d1632c1df088ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff2903391b07ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ee5b8dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642f9fe95bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f6306d7a6399f84ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606b7e29ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb886e485ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f55ab892ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648ed719ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fb096fcc022c4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a26f489ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf0334de0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e26fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b89c56d05335aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd065773160cf0ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e99fb6dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e035871f00aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6027bd89ef45cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf396373472932afULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c21281acb5ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef74308623fb379ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f50ab498abcab25ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fd60b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee31b12ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c490949ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe801171b0060c56ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9cf9d6b376968eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad3ef21ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dae3fdbULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe417ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789b904e8a810aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb89f5e2f49ecaULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9d948ada9e7aceULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc58cbc4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b78408f7d9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11d86d6051b3c8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb7f783cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2c1468ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af726220ee49ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850d9904ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7b970b4e6c7fbfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681c9d1a5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7cfd1bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97cd01c20aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce57e2aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdc5643b1ac0a47ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a7f38cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018db7dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d631c86fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c11271f51cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400798b7e7e08de1ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed3938aca4311acULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe89a3bab421d77eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d180458ac66ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c1c6213ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3effe9b7c617a6afULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d74e14ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235b27f4deULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7011d4334ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d1925e2e060ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6871521bce5946ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784d660eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502facfbdULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7154446901174dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6818481465fb1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc8d37b7dfc8cb4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eed8cfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a353230ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e06ed3cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363fd19965a4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400593641ae65a21ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f7de803f4dcdULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f57ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511d1f58ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f3575e1ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511da666ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511da664ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511da665ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f358f33ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e84928e8f711896ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0dbd84b00233cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8b0011a4fc01b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef8d21d234d3118ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb2ff29033a021aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf395c9e89ef955eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef42a642fa0e23aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f63071d7508bc58ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ec39c24606c70cdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0e0cb8878349ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e828fb8f55ba1e2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf047b64648fd641ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9fb0a8fd9ed7dcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb3d0782a27eb0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a748cf033f1a2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffada66e26fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b89c56cf30325ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebd066a131631cfULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed0e06b9e9b399dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2e0358743c2eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee50f1db8d9edb0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf396373472a6cd2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2f4c21280cdf6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef743086240d376ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4fe23e2374f76cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a0dd536fdff79ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbec3917deee32a89ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf07a0926c4a302dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe80117195a58d6aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9ce2355ecbaff0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9a74710ad492e1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb3c5bc8dae9ebaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018fffa100fe416ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d3907e0ac426ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ebb7dd1c9af5a8eULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9d962f4f5dc33cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8468ddc5a954aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370b7840a8ca9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11d9376c07cfa0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedbe57bdb80ce8cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5242c33e2cf65cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af726222298cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0d3ee2850f1eacULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7b97d8f16bd7ffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2c54e681ca78bdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e4babb7d7042ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee9ff97cd025b55ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e0b8e7ce6345bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdc56abafa2cdcbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f2d379ba1a8a59dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003668bc018d7c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eead055d6326bd5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6e81c11272b180ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400798c550084654ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed39398667b1a06ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe888c0770772eaeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee84d18045a7665ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea673202c1e0947ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe8b435d75ad47ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5247b845d83046ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbda235b50ed3dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10e2e7011e1449ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d1913510d65ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf67630972ef4d55ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2e9d1784dd953ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2c456502f8e7b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71544469018106ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee681845b2b8d0eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc7be377d59dccdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4003666269eed518ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2d27be1a349911ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7198839e075a48ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee7363faae87e76ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400581cb98600fccULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed1f7de77402afaULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511da665ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511da664ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f358f34ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511a7c30ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511a7c30ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd3dd0cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8557ce2511a7c3ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e845d2ce892100fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa0914a8faULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba32d5b19ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef884e6aec1719aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a75f866ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd9459f6d16ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac0fda986ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f62ca7e44d6bbf9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bb2f7cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8d72abULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf4aea7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd08512708398ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4cf060bd3f1a4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1cae12ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25dea828fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad202fd5a50ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa473ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb531e54c4be1ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece49f5ef4fc16eULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb31b45cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607f99e1aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68375a46a9ee0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fabe32aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331f9324ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec35027c9fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f510c12dbcff49aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a38270cdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed1efa5ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eed83b4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262e35fa0f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0c75621547887ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da942d8fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7ac1deULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b97845ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec26b2945c58ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2a94e166bcd9ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc937098494c8bULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9d48aaeb59fa4cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d7938342b782ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abad4b05ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11a13a79aa6eceULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe25a174ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d56724ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3beaacd9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd865552ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7b4265b5a6d903ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34026dc9dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d4156aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cd93c16ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f3c231ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdbfc611440e858ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade78bd4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6cd9cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aa77639ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26b92b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400efd1b3a4635dfULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee40a3c0aad51a6ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8a374e512a7d01ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6593c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10d1a044ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0051681802a37cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c85eeb0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc137b83ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0841f18ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d072aeb6810ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6900924177f0a9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d7b0bbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd0463c0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c10bb4baULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d9043c68da2bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc963a189347c3fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5d2d8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349c20fcULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122bd137a7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1f9bbfbc8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008fcb14bbf680eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23af00cc2d0d1ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511a7c30ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511a7c30ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c655f3894469f0cULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b033fULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b033dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd44256ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8581da9bcd4427ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e845df3f658e129ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa0916f30aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba32f72a6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef885905ea8acc4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a76cafaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a08b70ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac0fe8c6cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f62cabbd47a0211ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bc7b85ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8dfeabULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf5ee7bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd0851271f131ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4cf1f7570e650ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1db3d7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25deaf620ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad202fe9eeaULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa470ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb531e4af1572ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece4a07ff62dd93ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb33ff01ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607fbea4bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee59e5d44be0baeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fad01d4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331ebac6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec35038339ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f505cea49edd04cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a382fcc9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed1281dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eee5723ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262bd5d7a33ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0a4f186c73c0cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da94a120ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7a59b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b97847ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec268c30631aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07962968464aebaULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc87a7a467be5cULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9d4a2ff5984716ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d79383445d4bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abaedfd5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11a1f4cddc1ba8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe26cab9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d63555ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bebe823ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd87bb59ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7b43214e5b25c0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd340275b90ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d47b0eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cd9b092ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f462cdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdbfcbcc9a5ff3bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade80192ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6c321ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aa7e243ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26c1772ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400efd4112dcc734ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee40a48c3da54a4ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe893001eb2d25e0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6739faULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10d34783ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eff58e8ded4fa68ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c86bd18ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc3a020fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0859cb4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d0718578157ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf68022372c022ecULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d8167eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd02a0d7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c10c11faULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d904188f0542ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc85ec25d918eaeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5c85cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349b0cc5ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122bd1d199ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1dd945cfaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008c97938cd6b17ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23aef7944f6feULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b033eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b033dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f35109cULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b0342ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b0344ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd44259ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8581da9bcd4428ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8406c5ccf10a8bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa0916f32bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba32f5850ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef821ef105dea57ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a76cb1aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a08b81ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac0fe8d14ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f627e903ae2b785ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bc7ba7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8dfeb8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf68344ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd0851271f201ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4b1353ac64628ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1db3e7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25deaf626ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad202ff35f0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa474ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c51329ae9894eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece2ff4259d0cf9ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb34465bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607d99a39ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee68250887f8340ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469facfcfeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d63233206550ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34eb7311ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f510ba84518ff56ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a382fd0eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed33b0eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250edb6ef1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262e36052f9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0c74021a3a6daULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da94a131ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7bc721ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b9787fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec26b2951176ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2a94e276eca9ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc93156a37b28cULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9ccc40f751dc82ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d79383445d8aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abaeb9a5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1159f8a30235a7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe26c9f8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d6355fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bde03e2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd879f9eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7ad43ac8e9f0ecULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd340275bbcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d47b16ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cdc389bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f463b4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdb8b4c1daf4f70ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade801a3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6c322ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aa8dd59ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26c1804ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400ed0b4417e673bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3f7d1c4e56a40ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8a347b0acb1dc3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc67a1a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10bae948ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f00502632f472f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c86e014ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc14a213ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0629bfeULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d072aec45eeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf68ff4d648f801cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d812e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd04e293ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0fa24d8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d9043c68e202ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc96302c3010ff5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5c88aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349c9682ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122bb5a919ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1f9bc1016ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008fc9088b9bad8ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23ab2445cb830ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b0343ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b0344ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f35109dULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b8a51ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a51ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd4a7a5ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8501da9bcd4a7cULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e84078cdab7d312ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa09193d35ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3316fd5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef82298c0451e2eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a779daeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a1a9d8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac0ff6ffcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f627ecdca85fa29ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bdc7b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8e8ab6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf7c312ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd08512735f98ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4b14ea463396aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1eb9b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25deb69b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad203007a89ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa46fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c51329a4c52deULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece300625960988ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb3690f9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607dbe65fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee59d3828d3dbc1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fae1ba6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331f8cf0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34ec79a6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f505c7fb3379362ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a3838909ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed27384ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250edc425dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262bd5e2f2fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0a4db87168e89ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da9514c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7b5efcULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b9787cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec268c311b36ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07962968574de88ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc874c7e49acbcULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9ccdc601901c86ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d79383460353ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb04e75ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f115ab2f733dcfaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe27f336ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d7038eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bdf3f27ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8905a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7ad4f6619e383cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34027daaeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d4e0b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cdcad10ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f5044bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdb8ba7d31463eeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade87761ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6b8a5ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aa94961ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26c9cbcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400ed0da1a14f78bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3f7de7e132345ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe892d2ea4cec786ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6947d4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10bc9084ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eff566514b9e750ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c87ae7bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc3b289cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0641998ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d07185860a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6800de95d8be7fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d878a9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd031fa6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0fa8215ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d904188f0fe0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc85e23975f3b5bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5be0bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349b8249ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122bb64306ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1dd947378ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008c95875c7f4ffULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23ab1b0e5b881ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a51ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b8a51ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f3529f0ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b8a4fULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a4fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd4a7a2ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8501da9bcd4a7cULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e845ebb041fb26fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa09193d2eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3318a45ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef8863a0e8fe833ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a779d97ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a1a9d7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac0ff6f62ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f62caf9641d4850ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bdc79aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8e8ab4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf72e55ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd08512735ed6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4cf38df0ddb07ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1eb9a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25deb69b4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad202ffe38aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa470ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb531e411df02ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece4a19ef683260ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb3649b7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607fe3682ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee4b944e51157b3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fae2088ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331de274ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec350489d3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4f5b837017373aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a38388caULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed0609cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eef2a94ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262975b537cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0828cec39fc49ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da9514afULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e79f194ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b97843ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec2665cc69ddULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789a982762a09bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc7bdeb079d77bULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9d4bb4ffd6942cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d79383460326ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb074b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11a2af220dc8c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe27f403ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d7038fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bed2374ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd892174ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7b43dce70f72bbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34027da89ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d4e0baULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cda250bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f50370ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdbfd187f0b1646ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade87752ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6b8a7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aa84e4bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26c9c2fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400efd66eb73588dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee40a557d017efeULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8828b5852f9e6aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc68e03cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10d4eecbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0f018da47e9cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c878b87ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc6088afULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0871a5cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d0705c39aa8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6703b4a4083d5bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d87c44ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd00ddf4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c10c6f38ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d903f4b5305eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc759e331ee957aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5bde0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb3499f88eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122bd26b88ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1c16cbe2eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008964125db6bbaULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23aeee5ce37ceULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a4fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b8a4fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f3529efULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c115eULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c115eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd50cefULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8101da9bcd50d1ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e845f8211e683ccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa091b8748ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba333a1dcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef886e3be7723c4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a78702bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a2c837ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac1005252ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f62cb36f3c08e9bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bf13aaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8f16bbULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf86e2dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd0851274cc78ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4cf5248aacfc2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1fbf6eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25debdd48ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad20301282aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa46eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb531e374a893ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece4a2bef6d75e5ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb38945eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de6080082b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d42c856482d2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469faf3f33ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331d0a20ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec35059066ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4dfd324c52acf9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a38414c5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ecf991aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eeffe01ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b26271592cbfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0602851acb937ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da95883fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e798972ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b97841ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec263f6870a0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d299ca60927cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc7015b48cd6fdULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9d4d3a0a14e179ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d7938347a8f8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb20993ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11a369763f75fcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe291d49ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d7d1c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bee5ebcULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8a8787ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7b44987fc3bfd6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd340285979ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d54662ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cda9983ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f5a40fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdbfd7434702d67ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade8ed11ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6ae2cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aa8ba54ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26d20ebULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400efd8cc409e9eeULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee40a62362c0c2eULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8721691f31e686ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6a8673ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10d6960cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcc51a3c73d380ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c8859efULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc870f45ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b08897fbULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d06f32fb3f7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf660545d5503fe9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d8e206ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26ccff1b0fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c10ccc74ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d903d0db5b77ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc65504064b909dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5b35fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb3498e457ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122bd30574ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1a5451f60ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008630912e969f7ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23aee5265b7f9ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c115eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c115eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f354342ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c1161ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c115fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd50cf0ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c85d7ce2511c116ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e840853e87e9bd0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa091b874fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3338769ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef82342702c5220ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a78703cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a2c838ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac10052eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f627f0b5a293cd0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bf13bdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8f16bcULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf902e7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd0851274cd36ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4b1680e002cb0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1fbf77ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25debdd49ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad20301bf27ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa46eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c513299af1c6fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece301825a84704ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb38db9eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607de328bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee4b81fc928136cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469faf3a4dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331eb49bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34ed803cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4f5aae42ac2e0cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a3841502ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed1ac00ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250edd15c9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262975c0b65ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa08276ec8972eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da95884fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7af6d7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b97878ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec2665cd24f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789a982872d067ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc7b83824f61efULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9ccf4b0bce5cf2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d7938347a924ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb1e34bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f115b6d4b65846aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe291c74ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d7d1c3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3be07a6eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8a6bb2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7ad5b1fa527f9bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34028599eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d54661ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cdd2183ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f5a4e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdb8c038879786cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade8ed1fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6ae2cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aa9b567ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26d2178ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400ed0fff2ab87daULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3f7eb374dc23eULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8825e23ed240f6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6aee06ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10be37c0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0c7dc38ab978ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c887cdfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc61af31ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0659737ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d0705c47b61ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf67026fc721e50dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d8de68ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd015cc1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0fadf50ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d903f4b53dc3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc759446bbd5b1eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5b38bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349a6e13ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122bb6dcf2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1c16cd6dbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008962062d62cbdULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23ab11d6a3192ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1160ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c115fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c675f3894470459ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c9870ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9870ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd5723cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8501da9bcd5726ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e84091af64564b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa091dd16aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3359f01ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef823ec2013863dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a7942daULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a3e69aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac10135dcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f627f48e9cc7f87ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20c05fceULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8fa2c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebfa42bfULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd08512763ad6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4b181779d1fffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f20c541ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25dec50ddULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad2030303c4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa46bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c51329911e5ffULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece302a25ae4bdcULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb3b2650ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607e07ebeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d307697c2a43ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fb05900ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331ddc45ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34ee86d4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4dfc5d1ee91485ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a384a102ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed0e47cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250edde938ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b2627159e79cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0601251fc5401ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da95fbe0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7a8eb2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b97876ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec263f692eb0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d299cb70c246ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc6fba866e1c88ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9cd0d0160c9da9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d79383494ef8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb37829ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f115c279f972c02ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe2a45c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d89ff9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3be1b5bdULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8bd1c5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7ad66d9306c71aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34028d895ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d5ac0bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cdd9600ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f64589ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdb8c5f3dde8cf8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade962ddULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6a3afULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aaa216fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26da632ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400ed125cb42182fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3f7f7f082d2c1ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe871e95d8d58a0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6c9447ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10bfdf05ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcc296725b5c59ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c894b50ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc8835c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b06714d9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d06f330961fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf660400f86af3c4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d9442eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26ccff99dcULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0fb3c8dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d903d0db6ba7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc65465401b6f40ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5a90eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349959dbULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122bb776e0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1a5453a3eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400862e84fe46215ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23ab08a006074ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9870ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c9870ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f355c95ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b8a53ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a53ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd4a7a5ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8501da9bcd4a7cULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e83b05eb14ff4cfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa09193d40ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3315568ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7bef771fa54ceULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a779dc3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a1a9e0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac0ff7099ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f6232a230eeabedULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bdc7c3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8e8ab8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf857cfULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd08512736059ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4936469b897cbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1eb9b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25deb69b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad203011184ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa472ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bed335086c6bbULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece15f25bdec639ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb36d83cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607b99637ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6812b6c946766ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fae16c3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d63233213770ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34d4697aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f510b3dae620a01ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a3838946ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed4866aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250ec95a24ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262e36104f6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0c72a21f2d526ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da9514ceULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7ccc5bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b978b2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec26b295c68eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2a94e3871c78ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc92ba4c18b50eULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9c4fd70349a751ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d79383460382ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb02834ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1112b6cc59f23cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe27f267ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d7038eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bd15adbULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd88e9d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7a660fdc2cfe73ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34027dad4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d4e0bbULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cdf3510ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f50529ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdb1a37271db173ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade8776cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6b8a4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aaa4473ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26c9d48ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400ea44d48b69684ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3e5677f45eb98ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8a31a7c46bbdd3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc69af6cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10a4323dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f004ee44de64234ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c87d170ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc15c894ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b04118dbULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d072aed23bdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf68fe0887a70f61ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d8750fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd056159ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0e894efULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d9043c68e9d5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc96263fccda39eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5be38ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349d0c02ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122b9a1a81ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1f9bc2462ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008fc6fc5b40da0ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23a747bf84a14ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8a53ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511b8a53ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f3529f0ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c1163ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1161ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd50cf2ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8581da9bcd50d1ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e83b125bf16b4dfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa091b8757ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3336cf6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7bfa121e18158ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a787055ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a2c83dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac1005381ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f6232dfc091eb15ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bf13d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8f16bdULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebf997a3ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd0851274cdf4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4937dd35589a6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1fbf7dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25debdd4aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad20302561fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa46cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bed334fe9904bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece16045bd9141eULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb3922e5ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607bbe26eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee59c130ce9aba0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469faf356eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d63233205f17ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34d57016ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f505c151c815669ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a3841544ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed3bee6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eca2d95ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262bd5ee420ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0a4c58765e0feULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da958864ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7c643bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b978b7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec268c31d34cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079629686850e55ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc86f1501dcffaULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9c515c0d87dac7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d7938347a950ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb1bd07ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f111371208b9408ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe291babULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d7d1c3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bd2962cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8a4fe0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7a66cb74e14052ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd3402859c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d54663ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9cdfa98eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f5a5c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdb1a92dc82c389ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade8ed2cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6ae2bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aaab07cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26d2202ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400ea473214d25d0ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3e574388247a9ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe892a5b5e70689aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6b55a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10a5d97eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eff53e14a9ed3d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c889fd6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc3c4f23ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0429679ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d0718593fe7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf67ff99b8f159f1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d8dad0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd039e74ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0e8f231ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d904188f1a7cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc85d84d12ce801ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5b3baULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349bf7cdULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122b9ab475ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1dd9489f5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008c937b2c27ee2ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23a73e870a251ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1162ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c1161ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f354343ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c1165ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1163ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd50cf3ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8181da9bcd50d1ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8359f795aeceefULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa091b875dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba333528aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef75bffd396b13fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a78706dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a2c83eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac100541cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f61e6b426fa9949ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20bf13e5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8f16beULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebfa2c62ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd0851274ceb7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa4759398aae695ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f1fbf89ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25debdd4cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad20302ed20ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa471ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b893406240427ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdfbf0821f79a4ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb396a25ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607999248ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6800650a94b6cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469faf308dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d63233220992ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34bd5fe9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f510ad317ab14a7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a3841580ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed5d1caULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eb7455fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262e361b6f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0c71422420371ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da958872ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7dd19eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b978edULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec26b2967baaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2a94e4974c48ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc925f1dfa30c0ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9bd36d0f415aecULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d7938347a97dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb196c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10cb74f5b1a4b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe291adbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d7d1c1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bc4b1deULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8a3415ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf79f7e4ef7001c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd3402859eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d54663ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9ce2318dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f5a6a1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdaa922308c0e8cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade8ed3aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6ae2aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aabab91ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26d2291ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400e77e64feec3baULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3d2fd398a3dabULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8a2ed47e0c5d69ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6bbd36ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b108d7b37ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f004da268d8114cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c88c2c9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc16ef1bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b01f95b9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d072aee0190ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf68fcc3aabe9e94ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d8d735ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd05e025ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0d7050aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d9043c68f1a7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc961c5369a3746ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5b3e5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349d8186ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122b7e8bf4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1f9bc38aeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008fc4f02ae6068ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23a36b3905566ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c1164ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c1163ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f354343ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c9875ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9875ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd57240ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8501da9bcd5726ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e835abea375868fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa091dd183ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3356a22ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef75ca9837dd659ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a794308ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a3e6a5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac101370dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f61e6f1b69dd4e3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20c05ffaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8fa2c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebfb6c3eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd08512763c5bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa475ad0247d704ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f20c555ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25dec50dfULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad2030431c0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa468ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b89340586cdb7ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdfc0292254290ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb3bb4daULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de6079bde85ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee59aedf0ff7b64ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fb04f41ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d63233213144ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34be668aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f505baa85cb1970ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a384a180ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed50a4eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eb818d2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262bd5f9918ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0a4af87b53376ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da95fc02ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7d697dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b978e8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec268c328b64ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079629687953e25ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc869621ffb04cULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9bd4f2197f81d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d79383494f59ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb32ba3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10cc2f49e340e6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe2a4426ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d89ffaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bc5ed31ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8b9a27ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf79f8a088243e0fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34028d8e1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d5ac10ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9ce2a607ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f6473cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdaa97de5f11e10ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade962fcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6a3b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aac179bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26da74dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400e780c288551f8ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3d309f2b432d7ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8927881812092eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6d6377ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b108f2282ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eff515d8083c00eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c899138ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc3d75aeULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0211365ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d07185a1f34ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf67fe54dc09f553ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d93cfaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd041d43ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0d7624bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d904188f251aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc85ce60afa94abULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5a967ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349c6d52ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122b7f25e6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1dd94a071ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4008c916efbd08c8ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23a3620198838ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9875ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c9875ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f355c96ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c9873ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9873ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd5723fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8557ce2511c988ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e83b1ecccdd7515ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa091dd178ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3358494ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7c04ad1c8adf8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a7942f0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a3e6a2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac1013676ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f62331d50352a3fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20c05fe6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a8fa2c4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebfad781ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd08512763b9bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa493973cf27b85ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f20c54cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25dec50deULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad203039ac4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa46bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bed334f4c59dcULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece16165bea4ae1ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb3b6d94ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607be2ea5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee4b6faad3ecf0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fb0541dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331f86c5ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34d676b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4f59d9154124d8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a384a140ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed2f767ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250ecb0106ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262975cc350ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa08260ecd8e98dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da95fbf3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7bfc1aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b978b1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec2665cde008ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789a9829830033ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc7b28543d6189ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9c52e117c60e80ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d79383494f29ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb351e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f11142b74bd35f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe2a44f3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d89ffaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bd3d177ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8bb5f7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7a67870d958238ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34028d8b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d5ac0eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9ce01e05ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f64662ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdb1aee91e7d592ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade962edULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b6a3b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aab1c86ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26da6c0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400ea498f9e3b519ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3e580f1a0343dULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe88230ef874e305ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6cfbdeULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10a780c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe09f9f970f404ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c896e46ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc62d5b8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0441423ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d0705c55c1dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf67012aea3b8cacULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d94093ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd01db90ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0e94f6eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d903f4b54b2aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc758a5a58c20c4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5a939ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349ae397ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122b9b4e64ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1c16cef88ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400895ff9fd0edc0ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23a73550c3192ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c9873ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511c9873ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c655f389447261dULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511d1f82ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f80ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd5d788ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8581da9bcd5d7bULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e83b2b3daa43575ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa09201b8cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3379c23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7c0f481afda9bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a7a1584ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a504fdULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac1021961ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f62335adfd86965ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20c1abf1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a902ec9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebfc1751ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd0851277a935ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa493b0a68f6d61ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f21cb12ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25decc470ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad20304df5cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa469ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bed334eaf236cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ece16284bf05234ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb3db83aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607c07acbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d1e24d93d18bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fb172c8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331eae69ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34d77d43ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4dfb87f17f7bfdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a3852d3dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed22fdfULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250ecbd46fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262715aa277ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa05ffc524beec8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da966f80ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7b93f1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b978adULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec263f69ecbcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d299cc80f210ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc6f5f604f26cdULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9c54662204428fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d793834af4f4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb4e6bbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1114e5c8eed7e4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe2b6e38ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d96e2aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bd50cbeULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8d1c04ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7a6842a649c424ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd3402957adULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d611b3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9ce0927bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f6e701ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdb1b4a474ce79cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade9d8aaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b69935ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aab888bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26e2b79ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400ea4bed27a4463ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3e58daad000fbULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe871bc292792cfaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6ea213ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10a927fdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcc012a842e4d4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c8a3cacULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc895c44ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b04591b6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d06f3317849ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6602bc1b85a782ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d9a655ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd0018a8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0e9aca7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d903d0db7bd4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc653c679eb4dddULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f59ebeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb3499cf60ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122b9be84eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1a545551aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400862c78cdf5a32ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23a72c18bfd19ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f81ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511d1f80ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c66076a6f3575e9ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511d1f84ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f84ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd5d78dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8101da9bcd5d7bULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e835b85b13c3e4fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa09201b9bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba33781b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef75d533364fb93ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a7a15a1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a50505ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac10219fbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f61e72f4641108dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20c1ac0dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a902eccULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebfcac16ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd0851277a9fcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa475c66be4c776ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f21cb23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25decc474ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad203057663ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa467ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b893404e99748ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdfc14922b347dULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb3dff86ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de6079e2abaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee4b5d591558a78ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fb16df0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632332058ebULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34bf6d23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4f5903e7d61b8fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a3852d81ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed442d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eb8ec44ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262975d7b3cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa0824aed28602aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da966f98ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7d0162ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b978ecULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec2665ce9b23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789a982a932fffULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc7acd2e11abcdULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9bd67723bda8f7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d793834af528ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb4c07cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10cce99e14dd3cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe2b6d6fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7d96e31ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bc7287eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8d0035ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf79f95c20d87a79ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd3402957d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d611b7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9ce31a85ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f6e7dbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdaa9d99b562da9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbade9d8bcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b69934ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aac83a5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26e2c0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400e7832011be03eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3d316abe7be87ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe88203bb2178480ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc6f09b1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b1090c9c5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe07762f572e2aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c8a5fa6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc63fc3cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0229109ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d0705c63cd3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf66ffe60d55342cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8d9a2bfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd025a5eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0d7bf8cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d903f4b5588dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc75806df5ae664ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f59eecULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349b5920ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122b7fbfdcULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1c16d0839ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400895dedccbaebfULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23a358c96904cULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d1f84ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511d1f84ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f3575e9ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511da694ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511da694ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa01da9bcd63cd7ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8101da9bcd63cfULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e835c4cbf02f629ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfb9fa092265b7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8afffba3399951ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef75dfce34c20e6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb1dfae8a7ae838ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37dfd945a62368ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef2f97ac102fcedULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f61e76cd5e44c36ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edbaf2e20c2f818ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9884173a90bad1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a3827ebfdebe9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf1dd08512791798ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa475dfd581b7eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5749c9f22d0e7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb340b25ded3806ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9a7ad20306baf8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffd29a2aa468ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b8934044c60d9ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdfc2682305951ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecfc20ceb404a34ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2de607a076e7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d0bd31ab78b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf37e6469fb28c9cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb1d632331f8092ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5e2ec34c073bbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4dfab2c415e36aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f988375a385b97cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedba0120ed37b45ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2132250eb9bfadULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96b262715b5d5aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa05fe6529b8993ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fb34074da96e322ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee568ea8e7c9934ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ffcba1b978e5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe96ec263f6aaad1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d299cd9121dbULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecc6f0432306402ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9bd7fc2dfbd056ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6d793834c9afcULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea370a7abb65559ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10cda3f24679aaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed97054fe2c96b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f51309de7da3c63ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af5a3bc863c9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0b87cdcd8e6649ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf79fa17b98cb6efULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f28fbd34029d6caULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1c91240d6775fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee6ed9b9ce38efaULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c454373f7887fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdaaa3550bb3d41ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f449dfbadea4e76ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40024133d4b68eb8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f02eba10aacefa9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf863384a26eb0c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400e7857d9b26e80ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee3d3236510712dULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe8718ef4c1ccf77ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee6dd57dc70afedULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6730b10927106ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcbd8ede2a6cfdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5135490c8b2e0eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed96652cc8a82c9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fcac0b0240ea6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963d06f3325a7aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6601773ea05b29ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb1cddbf8da0882ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28ee26cd009777ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf704df5c0d81cc7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3d903d0db8c05ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc65327b3bb2c77ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400240bb80f5946eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf4492bb349a44e4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf899c122b8059c0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0060f1a5456ffaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400862a6c9da5250ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee23a34f91b9c2cULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511da694ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf857ce2511da694ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c64076a6f358f3cULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f982c7e9b3dcc55ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3dcc53ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fab8801f7e9ef2dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c90d10af7a372caULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e90179ce89313fbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5408f0ad09f6fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e93388f2029fa9aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf005b58ec58455cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb27295b61fc3cfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c554ba11e29c0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef681c334ea8de2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f664ad71513d4b3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee8efbbe768d330ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99da280d1f88d2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea785927d49c37cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bc1becce8e398ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3facd8ff8f528f03ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0d05eadec392ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdd4c2bd34201ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb6411877d23c6fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff7e46c29ea2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb46c41f5b1a7ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eda02f9701b7555ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed54595b8bd21a4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9632300b89bba9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee065fe151f59cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c5ceb17c36e5fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb26879feee7c3eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9f5776d904049ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54357eb1469602ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99d8fb3033a0e1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8e21d8cc4bdb1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf30022d22657174ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea4890c18543cdbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa5577e96dec4a1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdc9c64de6df2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefdfca2d2bd8152ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff6a5ca01850ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb343b03e2841beULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2964cf7d832dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed8bbb222ec5df4ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea72413eaf0111cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e81d408e1a823f0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee9a12ddf58ec1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeabada4d3b85b29ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f17859d9554965eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedd269168f7e78bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54667e256cfbd2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea0280a1f791ab9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f105606063276f5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf802dc0852fbb0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f287fad57476ecaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c3a286ec67faULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee675e835a0862aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6dcfb6c1fdaeadULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdd82eb0c223e5bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5082b55576642eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011c5a14477e0eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0e47da60cbb7f3ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf925f4ba6e6fe6dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x401317f1ddd6076cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ef0cf7dcb86ffafULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe94c5aa83db1f94ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeeea13fdf973542ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe81ce432004489aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeaff67576260ae2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f05c333d687b846ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f546c092caf51fbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edd1b88efbee61dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12dd5e07981e73ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9bfa1d32752827ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6da9a943f82d3bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c8aeec4fcbf6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28714b63e7020bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71311cf408f265ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3714cb5781a23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fcac100bac831d8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011b92f01d50fcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf507990fb83e19eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf95312b4d926300ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0a366b95a258e2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400c404bd9afc840ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeec371fe2f313dULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3dcc54ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf81dff82058434bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c85a215ef46e594ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f982c7e9b3e6445ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3e6445ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fab8801f7ea9c36ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c92c163f4d9f323ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e901838c51ddc64ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5408f0ad21fe6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e93388f202b77deULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf005bc63d9c5f67ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb27295b62083b4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c554ba11f7220ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef681c334eb91c1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f664b1ae9cb30b4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee8efbbe769f425ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99da280d2011f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea785927d4ad833ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bc1beccea3280ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3facd92d575cce02ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0d05eae02a91ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdd4c2bd3fb0aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb6411877d347a6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff7e46c29e98ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb46c403e087aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eda030787533abdULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed54595b8bea274ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9632300b8b5e68ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeecd4b5cfd3f39aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c5ceb17c4b71bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb26879feed3d99ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9f5776d916d26ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5365b7aab01f27ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99d8fb303429f6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8e21d8cc3d73dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf30022d2266332cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea4890bf4ff076aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa517756f5c0242ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdc9c64df26fcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefdfca2d2bca20cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff6a5ca0184eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb343b01d5471ceULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07961697cb56556ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed8b146543c4e91ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea7254582feee7cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e81d408e1a96675ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee9a12ddf7b824ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeabada4d3ba9b8fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f17869098d3db5aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedd269168f92985ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54667e256de847ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea0280a1f79e06bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f105606063345cbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf802e2c146f82caULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f287fad5747d48eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c3a286eccba8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee675e835a0e345ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6dcfb6c1fe4bc9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdd8344deb14451ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5082b55576bbc6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011c5a14476d42ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0e47da60cc5850ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf925f4ba6e768a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x401318102b303a8cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ef0cf8769fa7a36ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe93f500ee4c3aebULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeeea13fdf995f23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe81ce43202826bdULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeaff67576286588ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f04e71e633af9c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f546c092cb03eb1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edd1b88efec6027ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12dd5e0798f94aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9bfa1d1b06ea34ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c7bac00a1019cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c8aeec502fc0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28714b63da1151ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71311cf4094db9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3714c8fab1f7eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc9adef2a4e71a5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011b92f01d402eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf507990fb818321ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf95312b4d92de42ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0a366b670c1ff8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400beb8755bbb5eaULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeec3704e788fbcULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3e6445ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf81dff820558f23ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8382c7e9b3e645ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f982c7e9b3e6448ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3e644cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fab8801f7ea9c3cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c92d10af7a3765aULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8fa873fcceac5cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5408f0ad21ff7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e93388f202b651aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf001951c5663de2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb27295b6208417ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c554ba11f7233ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef681c334eb92a1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f65f0c3579b21a3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee8efbbe769f45aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99da280d2011fbULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea785927d4c366fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bc1beccea3351ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faca17ba6c6a30dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0d05eae02abcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdd4c2bd3fb1cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb6411877d43652ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff7e46c29ea0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c506e48e8b545ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed9ee356bb68dc9ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed54595b8bed27fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9632300b72f068ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee04e63f6b889aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c5ceb17c4ad42ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb26879feef3bf0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9f5776d74e3a8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5435093fcfdf6cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99d8fb30342abaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8e21d8cc5de25ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf30022d2254995cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea4890c1854d3f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa55756f55cd784ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdc9c64df274bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefdfca2d2bee7acULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff6a5ca018f1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb343b03e28d3bcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2964d276b44bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed8bb67963dace1ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6c30bd09ddbdfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e81d408e1a9742bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee9a12ddf7b874ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeabada4d3ba652cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f172688afe627f0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedd269168f928daULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54667e256de84dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea0280a1f712884ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10560606333456ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7fd8be8adc1bedULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f287fad5747d55eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c3a286eccbaeULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee675e835a5727dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6dcfb6c1fe4cddULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdd0bac4eee780aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5082b55576bbe9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011c5a14476d42ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0e47da60cf1945ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf925f4ba6e76926ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012f33359b4b633ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ef0c13cc9e24714ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe94c378da00a109ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeeea13fdf99ea65ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe81ce432005987cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeaff6757605b14cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f05c19005a5c654ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f546c092cb06532ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edd1b88efc022beULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12dd5e07842f0dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9bfa1d32763c60ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6da834bd965e53ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c8aeec502943ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28714b63e767b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71311cf3f66774ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3714cb5780942ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fcac06539997368ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011b92f01d40aaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf507990fb84390fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf95312b4d7b916cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0a366b95a26ac6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400c40176362540aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeec3126a01ce46ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3e644aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf81dff820558f12ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c7f442bde8dd967ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f982c7e9b3efc37ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3efc3bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fab8801f7eb4947ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c92d10af7a379eaULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8fa9abb5e43000ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5408f0ad3a067ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e93388f202ce255ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0019bf16aa5315ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb27295b62143f8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c554ba120ba8eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef681c334ec9679ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f65f1072c527999ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee8efbbe76b154eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99da280d209b18ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea785927d4d4b22ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bc1becceb8236ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faca1a96ed0df73ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0d05eae191bcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdd4c2bd4b424ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb6411877d54188ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff7e46c29e95ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c506e47310c17ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed9ee437ae06334ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed54595b8c05345ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9632300b749318ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeecd33c2deec1deULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c5ceb17c5f5f6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb26879feedfd45ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9f5776d761073ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f536542393a438aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99d8fb3034b3ccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8e21d8cc4f7aeULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf30022d22555b10ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea4890bf4ffa13eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa5174dcdda5884ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdc9c64dfe051ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefdfca2d2be0862ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff6a5ca018e6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb343b01d550658ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07961697fae966dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed8b0fbcb87f093ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6c43d68acaf76ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e81d408e1aab6a8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee9a12ddf9e1d1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeabada4d3bca588ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f17277bb365659eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedd269168fa6accULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54667e256ed4c1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea0280a1f71ee2eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10560606340326ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7fd995a95ba510ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f287fad57483b1dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c3a286ed2f59ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee675e835a5cf8eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6dcfb6c1fee9f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdd0c06217d7b94ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5082b555771383ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011c5a14475c79ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0e47da60cfb9a0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf925f4ba6e7d35cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012f351a70ee882ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ef0c146683d723bULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe93f2cf44728832ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeeea13fdf9c143aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe81ce4320297696ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeaff67576080bddULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f04e57a9259e737ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f546c092cb151deULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edd1b88efed9cbeULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12dd5e078509d8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9bfa1d1b080037ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c7a377a4071beULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c8aeec508d0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28714b63da76faULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71311cf3f6c2beULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3714c8fab1188ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc9ad53a920dbddULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011b92f01d2fdcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf507990fb81da93ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf95312b4d7c0cabULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0a366b670c3579ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400beb52df6e9cfbULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeec310ba4c9c0aULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3efc39ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf81dff82052dae1ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c85a215ef46f3d3ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511b8adbULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8ad9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faadc1ae5626cecULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c92c0ed4de6a542ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e832b7f7dcb3613ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdda2817a2c475ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8affbb59177827ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef7168b0451e7dcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ead681e7b01358fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf36796872201b0dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef1da72c775ed1dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f61aeb46b962535ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5b55032d761aeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97113c78a257d1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea4913b651418aaULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf281dac03aa4100ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa95e293aa4280dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcae07a134d182ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf251c0fd06dc3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb542bf28f41288ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff8460021700ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb478ff456967ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed6e596b5316d04ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecde13771c74ee0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2d9064814ba9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee37ff39bf21f5aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf367f746e1fa04cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbead580cba270ad5ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef4977617b78a4dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4d7bbb8e48116bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f971037128894e0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5a9754d8e8dccULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bd13cc56951a5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea1cdccbe23a68cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa2692b7116fdd7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf24748c3439f5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefc9e64600aa6a4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff71657fd76dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2678437e45b32ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789980a1d301b3ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5872a58afb258ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9b91da8d5ec8e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee57e1ecf2bcfe7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea3707961ca864fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f109a4d02c5a4b6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed72c65a292da32ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502e8c45041673ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af13afba5863ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f09ea7868c00a30ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf79ab0626cfbec4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f25f514ab2ba8d8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0be0c48661a44ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee4219c5179f9f0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a999c72255832ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fda55ba57c98f10ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502b3d9e3ed86aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012d100bfa013fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0da777feafc5ddULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf91f518f8d5ef4bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012e39ca8b93110ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eed9618b862991eULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe86bb7804e97df5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee58328ff5f8a86ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea672cd67b68533ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efc48a0181959b4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5032f1d3da5bd3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed723a23690d892ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0ded6efdddda85ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963cd21233b4f6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf65a7c0c6302743ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0c28d77aea9aaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf25e8389ae6b874ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6eae11fc53be26ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee16d2217195ff4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc5f49b70e6c69cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012c4d66cd0ee5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf50224c8e0f9865ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf94b6aecc9b3274ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf09ab95c6ce743aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400b6aef3d74e0d5ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeb03022c92b97eULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511b8adaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf848f946a764c4dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8381da9bcd4a83ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08ce80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c11e8ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c11eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faadc1ae56315c0ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c90c0ed4de6a86cULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e832c3674058653ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdda2817a4e086ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8affbb59198fc3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef71725b1fba6f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ead681e7b026950ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf36796872212018ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef1da72c776bbd3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f61aeea763714aeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5b55032d85dc3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97113c78a2d2b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea4913b65150bc9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf281dac03ab6448ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa95e511520f0b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcae07a13628a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf251c0fd12306ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb542bf28f5123fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff84600216ffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08cb478fda1e36eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed6e5a3bbd74b2fULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecde13771c96b6bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2d90648397d2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee2a84ab9945a68ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf367f746e20a59eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbead580cba257caaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef4977617b87972ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4c3223b48c1444ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97103712890fbcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5a9754d8dc806ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bd13cc56aa291ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea1cdcc9f770a65ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa23188bc499b4dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf24748c34ef23ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefc9e646009d3f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff71657fd762ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb267841891191cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d18516c38ae8ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed57e95401b27feULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9b93426a37d0e5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee57e1ecf2d5497ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea3707961cc1b2cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f109af8c578b1c8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed72c65a293db1cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502e8c4504d26eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af13afbb93a8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f09ea7868c15322ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf79abb11dd7d02fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f25f514ab2c054cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0be0c4866739aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee4219c517a4e86ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a999c7225e528ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fda560ad9e33eeaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502b3d9e3f2e92ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012d100bf9f0ccULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0da777feb06396ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf91f518f8d65751ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012e3ba55b9f4d9ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eed962a8e44ef37ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe85c39c43b30f6eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee58328ff610f8aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea672cd67b82c71ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efb12115a9b34edULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5032f1d3db1801ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed723a236b512bcULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0ded6efddf366aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963cd1ff9e1ea8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf64b83ddc61e96aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0c28d77af0318ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf25e8389adb7695ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6eae11fc5461cdULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee16d21f55eb3c8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc4ff2af9d5f0dbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012c4d66ccfe70ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf50224c8e0d6772ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf94b6aecc9bab26ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf09ab9599458ceeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400b1814ab254525ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeb0300891992f5ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c11e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf848f946a73a902ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8381da9bcd50d7ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c11ebULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c11ebULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faadc1ae56315c2ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c92abe71288e090ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e82da2a7cc8cc6fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdda2817a4e087ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8affbb59197547ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef6b9612ef764e8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ead681e7b0269deULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3679687221201bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef1da72c776bc5eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f616742f54c00c3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5b55032d85de6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97113c78a2d2b9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea4913b65162b06ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf281dac03ab64edULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa92dedaa30b5fbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcae07a13628b7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf251c0fd12304ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb542bf28f5eef4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff84600216fcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c507aedd3461aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed6d2127c096510ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecde13771c9aecbULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2d906461467dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee37ee877db2322ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf367f746e209df8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbead580cba283e28ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef4977617a1d65bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4d7b006bb7957fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f9710371289105aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5a9754d8f8969ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bd13cc54c0cefULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea1cdccbe242e1eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa26908f143650bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf24748c34ef69ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefc9e64600bfd15ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff71657fd7fdULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2678437e4ebf8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789980a4a95588ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed586e6261a7205ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9b1cbdc2ce0da8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee57e1ecf2d54beULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea3707961cbf4e4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f105738e244d85bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed72c65a293daa2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502e8c4504d26bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af13afadaff0ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f09ea7868c1373bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf79435132ceafb6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f25f514ab2c05f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0be0c4866739bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee4219c517e4698ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a999c7225e5f1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fd9eb53f2943e96ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502b3d9e3f2eb1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012d100bf9f0ccULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0da777feb30471ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf91f518f8d657cbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012bfb289c85ecbULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eed7b46d9290a21ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe86b8d9484dd919ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee58328ff617148ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea672cd679fcb27ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efc464eb3226a48ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5032f1d3db36a8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed723a23691d548ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0ded6efdbe3b90ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963cd212349491ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf65a698b68abf28ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0c28d77aefd6dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf25e8389ae714c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6eae11fc32a77dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee16d2217195763ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc5f4100fbbd967ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012c4d66ccfee5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf50224c8e0fee6aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf94b6aecc84e4f6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf09ab95c6ce8da2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400b6abbdcc2cc9cULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeb02a78b073d14ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c11ebULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf848f946a73a8f5ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8557ce2511c11fULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c840000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c98f9ULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c98f9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faadc1ae563be96ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c9280ed4de6ab96ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e82dae1730314c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdda2817a6fc8bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8affbb591b8cccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef6b9fbdca11d3aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ead681e7b039d9aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf36796872222525ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef1da72c7778b0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f616778ffeced13ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5b55032d959fdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97113c78a34da1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea4913b65171e28ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf281dac03ac8833ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa92e1584ad7c4fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcae07a1377fd7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf251c0fd1d841ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb542bf28f6eeacULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff84600216f3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c507aec2fc021ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed6d21f82af2e6aULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecde13771cbcb4cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2d906463929bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee2a73f957e3c8cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf367f746e21a349ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbead580cba26aff9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef4977617a2c57bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4c316891fcf3a7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97103712898b38ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5a9754d8ec39fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bd13cc54d5dd7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea1cdcc9f779455ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa231663c763d08ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf24748c35a4a0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefc9e64600b2a64ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff71657fd7f4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb267841891ac52ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d1851999deb7ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed57e510d92a3d7ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9b1e259fa709dfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee57e1ecf2ed965ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea3707961cd89b1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1057e4a4f7e04eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed72c65a294db88ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502e8c45058e63ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af13afaeeb32ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f09ea7868c2801cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7943fc29d6bc27ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f25f514ab2c6266ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0be0c4866ccf1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee4219c517e9b2fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a999c722672daULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fd9eba474adec59ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502b3d9e3f84deULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012d100bf9e05bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0da777feb3a233ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf91f518f8d6bfd3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012bfd036c921c2ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eed7b58aeff0882ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe85c0fd87185cffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee58328ff62f647ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea672cd67a1725bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efb0fbff5a5809dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5032f1d3dbf2cfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed723a236b60f69ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0ded6efdbf976eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963cd1ff9effa6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf64b715ccbd7e4bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0c28d77af56dcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf25e8389adbd2ebULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6eae11fc334b27ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee16d21f55eadc9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc4fe9f98ac0c55ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012c4d66ccee6fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf50224c8e0dbd73ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf94b6aecc855da8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf09ab959945a9dcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400b17e14a738a21ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeb02a5e79bea8eULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c98f9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf848f946a7105a7ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8301da9bcd572cULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f982c7e9b3efc3aULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3efc38ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fab8801f7eb4948ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c92d10af7a379eaULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f21ae28771772ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5408f0ad3a05fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e93388f202ccf79ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeffae953ce85a32ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb27295b621444dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c554ba120ba85ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef681c334ec9740ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f6596af9a226662ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee8efbbe76b1571ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99da280d209b12ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea785927d4ea94eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bc1becceb82f1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fac69f7be3ab1cbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0d05eae191d1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdd4c2bd4b423ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb6411877d63026ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff7e46c29e96ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bec704fdbb8e1ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed9d971674cea4bULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed54595b8c0833bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9632300b5c2506ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee036c9d851b44ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c5ceb17c5ec09ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb26879feeffb8cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9f5776d5986e4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f543493ce5928bfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99d8fb3034b483ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8e21d8cc6fe85ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf30022d2243c136ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea4890c18556afaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa5572f53daea5aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdc9c64dfe099ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefdfca2d2c04df6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff6a5ca01989ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb343b03e2965b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2964d56fe566ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed8bb1d117b6cd4ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea66203b64b9471ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e81d408e1aac448ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee9a12ddf9e1fbULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeabada4d3bc6efdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f16c773ca77ac24ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedd269168fa6a09ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54667e256ed4b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea0280a1f69363dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1056060633f1a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7f55fc0b58b5deULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f287fad57483be3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c3a286ed2f54ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee675e835aa5ec2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6dcfb6c1feeaf4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdc946d91baacc7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5082b5557713a1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011c5a14475c77ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0e47da60d27a8dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf925f4ba6e7d3daULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012ce74d593634eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ef0b2fbc8418033ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe94c147302621f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeeea13fdf9c9f5aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe81ce432006e841ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeaff67575e55780ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f05bfec34c3d410ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f546c092cb17850ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edd1b88efc15f3fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12dd5e07703f8eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9bfa1d32775073ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6da6c037348f33ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c8aeec508686ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28714b63e7cd55ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71311cf3e3dc72ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3714cb577f84fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fcabfc9b86ab4e9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011b92f01d3058ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf507990fb84907dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf95312b4d64bfd2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0a366b95a27ca2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400c3fe2ed14dfcfULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeec2b2d5c6de92ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3efc39ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf81dff82052dadcULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c83a215ef46f3d3ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f982c7e9b3f942bULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3f9429ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fab8801f7ebf654ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c930163f4d9fca2ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f22e5e18c8de9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5408f0ad520d9ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e93388f202e4cbcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeffaf6fdf707b0aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb27295b6220430ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c554ba12202e8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef681c334ed9b1fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f6596f36ed9ba5dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee8efbbe76c3668ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99da280d212434ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea785927d4fbe08ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bc1beccecd1d9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fac6a258644eb9eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0d05eae2f8d5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdd4c2bd56d36ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb6411877d73b60ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff7e46c29e8fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bec704e240fb4ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed9d97f7678936dULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed54595b8c2040aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9632300b5dc7c9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeecd1c28c098fd8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c5ceb17c734c7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb26879feeebce9ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9f5776d5ab3c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5364ccc7c467dcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99d8fb30353d98ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8e21d8cc61816ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf30022d224482f1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea4890bf5003b08ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa517262c58aeb7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdc9c64e099acULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefdfca2d2bf6ebaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff6a5ca0198eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb343b01d559ae0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079616982a7c784ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed8b0b13ebe8e47ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea663354e5a5e3aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e81d408e1ac06ccULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee9a12ddfc0b62ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeabada4d3beaf62ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f16c866cdf6e299ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedd269168fbac03ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54667e256fc12cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea0280a1f69fbf4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1056060634c078ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7f56d329d838baULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f287fad5748a1a6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c3a286ed9303ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee675e835aabbe5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6dcfb6c1ff8810ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdc94c76449adf7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5082b555776b3bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011c5a14474baeULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0e47da60d31aecULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf925f4ba6e83e0fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012ce9322ed94cfULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ef0b30566b2ade7ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe93f09d9a98d504ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeeea13fdf9ec93cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe81ce43202ac65eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeaff67575e7b22dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f04e3d6c178d45eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f546c092cb26506ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edd1b88efeed942ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12dd5e07711a69ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9bfa1d1b09162aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c78c2f3dfe1b3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c8aeec50ea51ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28714b63dadc9eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71311cf3e437caULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3714c8fab0389ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc9acb827f3460eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011b92f01d1f8bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf507990fb823204ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf95312b4d653b19ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0a366b670c4af6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400beb1e69218403ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeec2b12603d34eULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3f942aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf81dff8205026adULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8602c7e9b3f943ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a900000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f982c7e9b3f942eULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3f942eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fab8801f7ebf656ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c90910af7a37d7aULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8e9ae8541f69caULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5408f0ad520e4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e93388f202e39f5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeff2a86ef042624ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb27295b6220490ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c554ba12202f2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef681c334ed9bf8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f653c9bdca9a2f5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee8efbbe76c3697ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99da280d212438ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea785927d511c3fULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bc1beccecd2a2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fac3273d5aebb5aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0d05eae2f8f1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdd4c2bd56d38ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb6411877d82a04ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff7e46c29e93ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b887256cebc7eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed9c4ad5ae17a43ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed54595b8c23410ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9632300b4559c9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee01f2fb9eaddaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c5ceb17c72ae8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb26879fef0bb3eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9f5776d3e2a46ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54341e5ce2721bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99d8fb30353e54ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8e21d8cc81ef4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf30022d2232e91eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea4890c1856020dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa55707b258fd34ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdc9c64e099f2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefdfca2d2c1b44eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff6a5ca01a2dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb343b03e29f7acULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc07a2964d8691682ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed8bad284bf7a76ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea600fb9bf93b32ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e81d408e1ac147cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee9a12ddfc0ba8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeabada4d3be78f5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f16685ee509230cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedd269168fbab52ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54667e256fc12fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea0280a1f61440dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1056060634af01ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7ed3398bd543eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f287fad5748a271ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c3a286ed9306ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee675e835af4b18ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6dcfb6c1ff891fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdc1d2ed486dc81ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5082b555776b5cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011c5a14474baeULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0e47da60d5dbdeULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf925f4ba6e83e91ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012a9b651720ec2ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ef0a4bac699cb9dULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe94bf15864ba28eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeeea13fdf9f546fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe81ce4320083821ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeaff67575c4fdedULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f05be4863e1e1abULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f546c092cb28b7eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edd1b88efc29bdaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12dd5e075c502bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9bfa1d327864a3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6da54bb0d2c00eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c8aeec50e3d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28714b63e83300ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71311cf3d15184ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3714cb577e767ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fcabf2e373bf673ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011b92f01d2004ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf507990fb84e7edULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf95312b4d4dee41ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0a366b95a28e81ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400c3fae76c76b95ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeec2534188e11dULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b3f942eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf81dff8205026a2ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c832215ef46faf3ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079c80000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f982c7e9b402c1dULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b402c1dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fab8801f7eca364ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c92910af7a38109ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8e9c200d34d312ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5408f0ad6a156ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e93388f202fb731ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeff2b61918c3d64ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb27295b622c46fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c554ba1234b4dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef681c334ee9fd3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f653cdfb160f2f0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee8efbbe76d578bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99da280d21ad56ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea785927d5230f2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bc1beccee2189ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fac32a19db8f28cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efe0d05eae45ffdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdd4c2bd6264dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb6411877d93541ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff7e46c29e89ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b887255171350ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed9c4bb720c515aULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed54595b8c3b4d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9632300b46fc7cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeecd048ea245db8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf3c5ceb17c8739aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb26879feef7c94ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef9f5776d3f5716ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f536457564e8c32ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f99d8fb3035c765ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee8e21d8cc73883ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf30022d2233aad4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea4890bf500d4d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa516fe8ad704f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbfdc9c64e152feULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefdfca2d2c0d512ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff6a5ca01a25ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb343b01d562f67ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079616985a0f89dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed8b066b603c76eULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea6022d3407fb2fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e81d408e1ad56faULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeee9a12ddfe3505ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeabada4d3c0b951ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f166951e888523dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbedd269168fced41ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f54667e2570ada0ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea0280a1f6209b8ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f10560606357dd2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7ed410aa54c07fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f287fad5749082eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c3a286edf6b2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee675e835afa82cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6dcfb6c2002631ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fdc1d88a715db53ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5082b55577c2f5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011c5a14473ae2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0e47da60d67c37ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf925f4ba6e8a8c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4012a9d49ecc3f6eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ef0a4c46502066cULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe93ee6bf0bf217aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeeea13fdfa17e45ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe81ce43202c1634ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeaff67575c75883ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f04e232f097c15aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f546c092cb3782fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3edd1b88eff015d2ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f12dd5e075d2af7ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e9bfa1d1b0a2c25ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6c774e6d7f519aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb2c8aeec514796ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf28714b63db4244ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf71311cf3d1acd2ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee3714c8faaf590ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc9ac1ca6c5b043ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40011b92f01d0f35ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf507990fb828974ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf95312b4d4e6982ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf0a366b670c6072ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400beae9f2d46b12ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeec25191e9f5efULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f882c7e9b402c1dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf81dff8204d726bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c852215ef470212ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511c98fbULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c98fdULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faadc1ae563be9bULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c90c0ed4de6ab96ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8288d57bc653d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdda2817a6fc9cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8affbb591b726aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef65c37599cd4cfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ead681e7b039e34ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf36796872222529ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef1da72c7778b9cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f611fd17f01d5c9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5b55032d95a21ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97113c78a34da1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea4913b65183d68ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf281dac03ac88daULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa8fdb219bd3f5aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcae07a1377ff1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf251c0fd1d845ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb542bf28f7cb67ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff84600216f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bec7cdc6122cfULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed6be8e42d52a95ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecde13771cc0ec3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2d906441415dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee37ddd53c426c2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf367f746e219ba8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbead580cba297184ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef49776178c2272ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4d7a454927198dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97103712898bdaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5a9754d908508ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bd13cc52ec846ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea1cdccbe24b5b1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa268e6716fcc40ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf24748c35a4eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefc9e64600d538dULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff71657fd893ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2678437e57cc1ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789980a77fa95dULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed586a1f798f714ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9aa7a0f83d3ce5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee57e1ecf2ed994ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea3707961cd6375ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f101424c1c4028eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed72c65a294db1bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502e8c45058e63ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af13afa1078bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f09ea7868c2644aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf78db9c3ecd9731ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f25f514ab2c6316ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0be0c4866ccf1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee4219c5182934bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a999c722673adULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fd980ed8d5ee99eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502b3d9e3f84faULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012d100bf9e058ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0da777feb6430dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf91f518f8d6c04cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40129bc86ad78ae6ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eed6074f9f2582eULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe86b63a8bb233bfULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee58328ff63580fULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea672cd6789111dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efc43fd4e2b7a93ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5032f1d3dc117eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed723a23692d1fcULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0ded6efd9e9cabULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963cd212357424ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf65a570a6e556f8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0c28d77af5134ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf25e8389ae77119ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6eae11fc1190e5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee16d2217194ecaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc5f384ae90ec34ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012c4d66cceee6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf50224c8e10446eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf94b6aecc6e9781ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf09ab95c6cea708ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400b6a887c10b864ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeb024ce9935124ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511c98fcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf848f946a710595ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8581da9bcd572cULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08c200000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511d200aULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d200aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faadc1ae564676aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c9280ed4de6aec1ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e82898c72009440ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdda2817a9189cULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8affbb591d89eaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef65cd207468660ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ead681e7b04d1e8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf36796872232a32ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef1da72c7785a48ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f61200789a2beeaULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5b55032da5632ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97113c78a3c889ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea4913b65193084ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf281dac03adac1bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa8fdd9f43a036bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcae07a138d70dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf251c0fd28d82ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb542bf28f8cb18ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff84600216efULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bec7cdabd9cd6ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed6be9b417ac954ULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecde13771ce2b38ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2d9064438d74ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee2a63471681e98ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf367f746e22a0f6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbead580cba27e350ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef49776178d1192ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4c30ad6f6dd308ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f971037128a06b5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5a9754d8fbf3aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bd13cc5301926ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea1cdcc9f781e46ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa23143bca2dec0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf24748c365a1dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefc9e64600c80d6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff71657fd88bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2678418923f84ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d1851c703287ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed57e0ce3092c18ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9aa908d5162d4fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee57e1ecf305e39ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea3707961cef842ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f1014d084770560ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed72c65a295dbfcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502e8c45064a58ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af13afa242c4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f09ea7868c3ad26ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf78dc4735d59e9fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f25f514ab2cbf86ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0be0c48672646ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee4219c5182e7dcULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a999c7227009aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fd9813e0f789540ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502b3d9e3fdb25ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012d100bf9cfe8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0da777feb6e0c5ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf91f518f8d72852ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40129be617d84d0fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eed6086cfb414f1ULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe85be5eca7daa22ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee58328ff64dd08ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea672cd678ab851ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efb0d6e90afcc0fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5032f1d3dccda3ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed723a236b70c1aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0ded6efd9ff882ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963cd1ff9fe0aeULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf64b5edbd19131eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0c28d77afaaa0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf25e8389adc2f3cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6eae11fc123488ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee16d21f55ea7d5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc4fe14378227d2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012c4d66ccde71ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf50224c8e0e1377ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf94b6aecc6f102dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf09ab959945c6c9ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400b17ade9c1cf1aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeb024b460372d0ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d200aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf848f946a6e624eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8501da9bcd5d81ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc079000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511d200cULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d200cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faadc1ae5646771ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8d01da9bcd5d81ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8237807ac3cc42ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdda2817a918a4ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8affbb591d6f7cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5ff0d844237baULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ead681e7b04d281ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf36796872232a35ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef1da72c7785ad8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f60d86008b7a460ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5b55032da565dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97113c78a3c88bULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea4913b651a4fcaULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf281dac03adacc7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa8cd768949c425ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcae07a138d730ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf251c0fd28d8cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb542bf28f9a7daULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff84600216f0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b887ecaeeff82ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed6ab0a09a6962fULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecde13771ce6eaaULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2d9064213c34ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee37cd22fad2a3eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf367f746e229956ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbead580cba2aa4d3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef4977617766e88ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4d798a26969d82ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f971037128a075aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5a9754d9180a6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bd13cc511839bULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea1cdccbe253d42ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa268c3f19c336aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf24748c365a6cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefc9e64600eaa06ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff71657fd92dULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb2678437e60d8cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc0789980aa55fd33ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed5865dc90f1124ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9a32842dac5689ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee57e1ecf305e65ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea3707961ced1ffULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fa221428646bcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed72c65a295db8cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502e8c45064a55ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af13af945f21ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f09ea7868c3914bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf7873e74acc754fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f25f514ab2cc038ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0be0c48672648ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee4219c5186dff6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a999c72270166ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fd916872829904eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502b3d9e3fdb47ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012d100bf9cfe8ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0da777feb981acULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf91f518f8d728d0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x401277de4be6b55fULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eed45a31a94166bULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe86b39bcf168dd8ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee58328ff653ed1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea672cd67725714ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efc41abe9348a74ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5032f1d3dcec53ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed723a23693ceacULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0ded6efd7efdbfULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963cd2123653b4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf65a448973feeabULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0c28d77afa4fbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf25e8389ae7cd6cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6eae11fbf07a48ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee16d2217194634ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc5f2f94d65fefcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012c4d66ccdeebULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf50224c8e109a76ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf94b6aecc584a0eULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf09ab95c6cec074ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400b6a551b5ea427ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeb01f247f07069ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511d200cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf848f946a6e623eULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c8301da9bcd5d81ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    {
        ExactBoxType domain(6u);
        domain[0u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp));
        domain[1u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08bbc0000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b580000000000ULL})), dp));
        domain[2u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc078380000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077700000000000ULL})), dp));
        domain[3u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[4u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4014000000000000ULL})), dp));
        domain[5u] = ExactIntervalType(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f957ce2511da71aULL})), dp));
        Vector<ValidatedTaylorModelDP> state_models(5u, ValidatedTaylorModelDP(6u, Sweeper<FloatDP>(new TrivialSweeper<FloatDP>(dp))));
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511da718ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3faadc1ae5651042ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c90ebe71288ed39ULL})), dp)));
            state_models[0u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e82383770fe04c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecdda2817ab34a1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8affbb591f86feULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef5ffa831ebe26cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ead681e7b060639ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf36796872242f36ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef1da72c7792985ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f60d89613588a47ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee5b55032db526bULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f97113c78a4436eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ea4913b651b42e1ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf281dac03aed007ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa8cd9e63c685f4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efcae07a13a2e46ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf251c0fd342c4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eb542bf28faa788ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff84600216ebULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc08b887ec94b798aULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed6ab17085266cbULL})), dp)));
            state_models[1u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ecde13771d08b21ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e8f2d9064238849ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee2a5294d520075ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf367f746e239ea0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbead580cba2916a1ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbef4977617775da4ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f4c2ff24cdeb257ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f971037128a8232ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee5a9754d90bad3ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf2bd13cc512d476ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea1cdcc9f78a839ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fa231213ccf8076ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fbf24748c370f9eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbefc9e64600dd74cULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x4018ff71657fd923ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbeb267841892d2c0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xc077d1851f468659ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed57dc8b079e252ULL})), dp)));
            state_models[2u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe9a33ec0a853b3aULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee57e1ecf31e304ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea3707961d066cbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0fa378c7ec420cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbed72c65a296dc6eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502e8c45070648ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe99af13af959a57ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f09ea7868c4da2dULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf78749241d477b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f25f514ab2d1ca7ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0be0c48677f99ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ee4219c5187348aULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6a999c72278e55ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fd916d7aa4339beULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f502b3d9e40316eULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012d100bf9bf74ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0da777feba1f61ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf91f518f8d790d5ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x401277fbf8e776baULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eed45b4f08909acULL})), dp)));
            state_models[3u] = std::move(tm);
        }
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 4u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbe85bbc00de2f6c0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee58328ff66c3c6ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbea672cd6773fe42ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 3u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3efb0b1d2bba171fULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f5032f1d3dda873ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3ed723a236b808c4ULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f0ded6efd805991ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e963cd1ffa0c1b0ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 2u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf64b4c5ad74a7d2ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbfb0c28d77affe65ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf25e8389adc8b8eULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf6eae11fbf11decULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbee16d21f55ea1dbULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3fc4fd88d658434cULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 1u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x40012c4d66ccce73ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 1u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf50224c8e0e697aULL})), dp);
            tm[MultiIndex{0u, 0u, 1u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf94b6aecc58c2b6ULL})), dp);
            tm[MultiIndex{0u, 1u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf09ab959945e3bcULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x400b177a89101416ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3eeb01f0a4764510ULL})), dp)));
            state_models[4u] = std::move(tm);
        }
        ValidatedVectorMultivariateTaylorFunctionModelDP state_map(domain, state_models);
        List<ValidatedConstraint> constraints;
        {
            ValidatedTaylorModelDP tm(6u, Sweeper<FloatDP>(new ThresholdSweeper<FloatDP>(dp, FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3e80000000000000ULL})), dp))));
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 1u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3f857ce2511da719ULL})), dp);
            tm[MultiIndex{0u, 0u, 0u, 0u, 0u, 0u}] = FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xbf848f946a6bbef6ULL})), dp);
            tm.set_error(FloatDPError(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x3c85d7ce2511da72ULL})), dp)));
            const ValidatedScalarMultivariateTaylorFunctionModelDP fn(domain, tm);
            constraints.append(ValidatedConstraint(ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0xfff0000000000000ULL})), dp))), ValidatedScalarMultivariateFunction(fn), ValidatedNumber(FloatDPBounds(FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp), FloatDP(cast_exact(std::bit_cast<double>(std::uint64_t{0x0ULL})), dp)))));
        }
        sets.emplace_back(domain, ValidatedVectorMultivariateFunctionModelDP(state_map), constraints);
    }
    return sets;
}
} // namespace Ariadne::SPRE22Fixture
#endif
