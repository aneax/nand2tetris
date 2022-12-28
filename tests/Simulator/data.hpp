#ifndef HARDWARESIMULATOR_TEST_DATA_HPP
#define HARDWARESIMULATOR_TEST_DATA_HPP

#include "HardwareSimulator/Bit.hpp"
#include <tuple>
#include <vector>

namespace test::memory
{
using InputType  = std::tuple<int, int, int>;
using OutputType = int;

namespace ram64
{
static const std::vector<InputType> Inputs{
  {     0, 0,  0},
  {     0, 0,  0},
  {     0, 1,  0},
  {  1313, 0,  0},
  {  1313, 1, 13},
  {  1313, 0,  0},
  {  4747, 0, 47},
  {  4747, 1, 47},
  {  4747, 0, 47},
  {  4747, 0, 13},
  {  6363, 0, 13},
  {  6363, 1, 63},
  {  6363, 0, 63},
  {  6363, 0, 47},
  {  6363, 0, 63},
  {  6363, 0, 40},
  {  6363, 0, 41},
  {  6363, 0, 42},
  {  6363, 0, 43},
  {  6363, 0, 44},
  {  6363, 0, 45},
  {  6363, 0, 46},
  {  6363, 0, 47},
  { 21845, 1, 40},
  { 21845, 1, 41},
  { 21845, 1, 42},
  { 21845, 1, 43},
  { 21845, 1, 44},
  { 21845, 1, 45},
  { 21845, 1, 46},
  { 21845, 1, 47},
  { 21845, 0, 40},
  { 21845, 0, 41},
  { 21845, 0, 42},
  { 21845, 0, 43},
  { 21845, 0, 44},
  { 21845, 0, 45},
  { 21845, 0, 46},
  { 21845, 0, 47},
  {-21846, 1, 40},
  {-21846, 0, 40},
  {-21846, 0, 41},
  {-21846, 0, 42},
  {-21846, 0, 43},
  {-21846, 0, 44},
  {-21846, 0, 45},
  {-21846, 0, 46},
  {-21846, 0, 47},
  { 21845, 1, 40},
  {-21846, 1, 41},
  {-21846, 0, 40},
  {-21846, 0, 41},
  {-21846, 0, 42},
  {-21846, 0, 43},
  {-21846, 0, 44},
  {-21846, 0, 45},
  {-21846, 0, 46},
  {-21846, 0, 47},
  { 21845, 1, 41},
  {-21846, 1, 42},
  {-21846, 0, 40},
  {-21846, 0, 41},
  {-21846, 0, 42},
  {-21846, 0, 43},
  {-21846, 0, 44},
  {-21846, 0, 45},
  {-21846, 0, 46},
  {-21846, 0, 47},
  { 21845, 1, 42},
  {-21846, 1, 43},
  {-21846, 0, 40},
  {-21846, 0, 41},
  {-21846, 0, 42},
  {-21846, 0, 43},
  {-21846, 0, 44},
  {-21846, 0, 45},
  {-21846, 0, 46},
  {-21846, 0, 47},
  { 21845, 1, 43},
  {-21846, 1, 44},
  {-21846, 0, 40},
  {-21846, 0, 41},
  {-21846, 0, 42},
  {-21846, 0, 43},
  {-21846, 0, 44},
  {-21846, 0, 45},
  {-21846, 0, 46},
  {-21846, 0, 47},
  { 21845, 1, 44},
  {-21846, 1, 45},
  {-21846, 0, 40},
  {-21846, 0, 41},
  {-21846, 0, 42},
  {-21846, 0, 43},
  {-21846, 0, 44},
  {-21846, 0, 45},
  {-21846, 0, 46},
  {-21846, 0, 47},
  { 21845, 1, 45},
  {-21846, 1, 46},
  {-21846, 0, 40},
  {-21846, 0, 41},
  {-21846, 0, 42},
  {-21846, 0, 43},
  {-21846, 0, 44},
  {-21846, 0, 45},
  {-21846, 0, 46},
  {-21846, 0, 47},
  { 21845, 1, 46},
  {-21846, 1, 47},
  {-21846, 0, 40},
  {-21846, 0, 41},
  {-21846, 0, 42},
  {-21846, 0, 43},
  {-21846, 0, 44},
  {-21846, 0, 45},
  {-21846, 0, 46},
  {-21846, 0, 47},
  { 21845, 1, 47},
  { 21845, 0, 40},
  { 21845, 0, 41},
  { 21845, 0, 42},
  { 21845, 0, 43},
  { 21845, 0, 44},
  { 21845, 0, 45},
  { 21845, 0, 46},
  { 21845, 0, 47},
  { 21845, 0,  5},
  { 21845, 0, 13},
  { 21845, 0, 21},
  { 21845, 0, 29},
  { 21845, 0, 37},
  { 21845, 0, 45},
  { 21845, 0, 53},
  { 21845, 0, 61},
  { 21845, 1,  5},
  { 21845, 1, 13},
  { 21845, 1, 21},
  { 21845, 1, 29},
  { 21845, 1, 37},
  { 21845, 1, 45},
  { 21845, 1, 53},
  { 21845, 1, 61},
  { 21845, 0,  5},
  { 21845, 0, 13},
  { 21845, 0, 21},
  { 21845, 0, 29},
  { 21845, 0, 37},
  { 21845, 0, 45},
  { 21845, 0, 53},
  { 21845, 0, 61},
  {-21846, 1,  5},
  {-21846, 0,  5},
  {-21846, 0, 13},
  {-21846, 0, 21},
  {-21846, 0, 29},
  {-21846, 0, 37},
  {-21846, 0, 45},
  {-21846, 0, 53},
  {-21846, 0, 61},
  { 21845, 1,  5},
  {-21846, 1, 13},
  {-21846, 0,  5},
  {-21846, 0, 13},
  {-21846, 0, 21},
  {-21846, 0, 29},
  {-21846, 0, 37},
  {-21846, 0, 45},
  {-21846, 0, 53},
  {-21846, 0, 61},
  { 21845, 1, 13},
  {-21846, 1, 21},
  {-21846, 0,  5},
  {-21846, 0, 13},
  {-21846, 0, 21},
  {-21846, 0, 29},
  {-21846, 0, 37},
  {-21846, 0, 45},
  {-21846, 0, 53},
  {-21846, 0, 61},
  { 21845, 1, 21},
  {-21846, 1, 29},
  {-21846, 0,  5},
  {-21846, 0, 13},
  {-21846, 0, 21},
  {-21846, 0, 29},
  {-21846, 0, 37},
  {-21846, 0, 45},
  {-21846, 0, 53},
  {-21846, 0, 61},
  { 21845, 1, 29},
  {-21846, 1, 37},
  {-21846, 0,  5},
  {-21846, 0, 13},
  {-21846, 0, 21},
  {-21846, 0, 29},
  {-21846, 0, 37},
  {-21846, 0, 45},
  {-21846, 0, 53},
  {-21846, 0, 61},
  { 21845, 1, 37},
  {-21846, 1, 45},
  {-21846, 0,  5},
  {-21846, 0, 13},
  {-21846, 0, 21},
  {-21846, 0, 29},
  {-21846, 0, 37},
  {-21846, 0, 45},
  {-21846, 0, 53},
  {-21846, 0, 61},
  { 21845, 1, 45},
  {-21846, 1, 53},
  {-21846, 0,  5},
  {-21846, 0, 13},
  {-21846, 0, 21},
  {-21846, 0, 29},
  {-21846, 0, 37},
  {-21846, 0, 45},
  {-21846, 0, 53},
  {-21846, 0, 61},
  { 21845, 1, 53},
  {-21846, 1, 61},
  {-21846, 0,  5},
  {-21846, 0, 13},
  {-21846, 0, 21},
  {-21846, 0, 29},
  {-21846, 0, 37},
  {-21846, 0, 45},
  {-21846, 0, 53},
  {-21846, 0, 61},
  { 21845, 1, 61},
  { 21845, 0,  5},
  { 21845, 0, 13},
  { 21845, 0, 21},
  { 21845, 0, 29},
  { 21845, 0, 37},
  { 21845, 0, 45},
  { 21845, 0, 53},
  { 21845, 0, 61},
  {    -1, 1,  1},
  {    -2, 1,  8},
  {    -3, 1, 16},
  {    -4, 1, 24},
  {    -5, 1, 32},
  {    -6, 1, 40},
  {    -7, 1, 48},
  {    -8, 1, 56},
};

static const std::vector<OutputType> Results = {
  0,      0,      0,      0,      1313,   0,      0,      4747,   4747,
  1313,   1313,   6363,   6363,   4747,   6363,   0,      0,      0,
  0,      0,      0,      0,      4747,   21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  21845,  21845,  21845,  -21846, -21846, 21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  -21846, 21845,  -21846, 21845,  21845,
  21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  -21846,
  21845,  21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,
  21845,  -21846, 21845,  21845,  21845,  21845,  21845,  -21846, 21845,
  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  -21846,
  21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  21845,
  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  -21846, 21845,
  21845,  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  21845,
  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  21845,  0,      1313,   0,      0,      0,      21845,  0,      0,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  -21846, -21846,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  -21846,
  21845,  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  21845,
  -21846, 21845,  21845,  -21846, 21845,  21845,  21845,  21845,  21845,
  21845,  -21846, 21845,  21845,  21845,  -21846, 21845,  21845,  21845,
  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  -21846, 21845,
  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  21845,
  -21846, 21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,
  21845,  21845,  -21846, 21845,  21845,  -21846, 21845,  21845,  21845,
  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  21845,  -1,     -2,     -3,     -4,
  -5,     -6,     -7,     -8,
};
}   //namespace ram64

namespace ram512
{
static const std::vector<InputType> Inputs = {
  {    0, 0,   0},
  {    0, 0,   0},
  {    0, 1,   0},
  {13099, 0,   0},
  {13099, 1, 130},
  {13099, 0,   0},
  { 4729, 0, 472},
  { 4729, 1, 472},
  { 4729, 0, 472},
  { 4729, 0, 130},
  { 5119, 0, 130},
  { 5119, 1, 511},
  { 5119, 0, 511},
  { 5119, 0, 472},
  { 5119, 0, 511},
  { 5119, 0, 168},
  { 5119, 0, 169},
  { 5119, 0, 170},
  { 5119, 0, 171},
  { 5119, 0, 172},
  { 5119, 0, 173},
  { 5119, 0, 174},
  { 5119, 0, 175},
  {21845, 1, 168},
  {21845, 1, 169},
  {21845, 1, 170},
  {21845, 1, 171},
  {21845, 1, 172},
  {21845, 1, 173},
  {21845, 1, 174},
  {21845, 1, 175},
  {21845, 0, 168},
  {21845, 0, 169},
  {21845, 0, 170},
  {21845, 0, 171},
  {21845, 0, 172},
  {21845, 0, 173},
  {21845, 0, 174},
  {21845, 0, 175},
  {21846, 1, 168},
  {21846, 0, 168},
  {21846, 0, 169},
  {21846, 0, 170},
  {21846, 0, 171},
  {21846, 0, 172},
  {21846, 0, 173},
  {21846, 0, 174},
  {21846, 0, 175},
  {21845, 1, 168},
  {21846, 1, 169},
  {21846, 0, 168},
  {21846, 0, 169},
  {21846, 0, 170},
  {21846, 0, 171},
  {21846, 0, 172},
  {21846, 0, 173},
  {21846, 0, 174},
  {21846, 0, 175},
  {21845, 1, 169},
  {21846, 1, 170},
  {21846, 0, 168},
  {21846, 0, 169},
  {21846, 0, 170},
  {21846, 0, 171},
  {21846, 0, 172},
  {21846, 0, 173},
  {21846, 0, 174},
  {21846, 0, 175},
  {21845, 1, 170},
  {21846, 1, 171},
  {21846, 0, 168},
  {21846, 0, 169},
  {21846, 0, 170},
  {21846, 0, 171},
  {21846, 0, 172},
  {21846, 0, 173},
  {21846, 0, 174},
  {21846, 0, 175},
  {21845, 1, 171},
  {21846, 1, 172},
  {21846, 0, 168},
  {21846, 0, 169},
  {21846, 0, 170},
  {21846, 0, 171},
  {21846, 0, 172},
  {21846, 0, 173},
  {21846, 0, 174},
  {21846, 0, 175},
  {21845, 1, 172},
  {21846, 1, 173},
  {21846, 0, 168},
  {21846, 0, 169},
  {21846, 0, 170},
  {21846, 0, 171},
  {21846, 0, 172},
  {21846, 0, 173},
  {21846, 0, 174},
  {21846, 0, 175},
  {21845, 1, 173},
  {21846, 1, 174},
  {21846, 0, 168},
  {21846, 0, 169},
  {21846, 0, 170},
  {21846, 0, 171},
  {21846, 0, 172},
  {21846, 0, 173},
  {21846, 0, 174},
  {21846, 0, 175},
  {21845, 1, 174},
  {21846, 1, 175},
  {21846, 0, 168},
  {21846, 0, 169},
  {21846, 0, 170},
  {21846, 0, 171},
  {21846, 0, 172},
  {21846, 0, 173},
  {21846, 0, 174},
  {21846, 0, 175},
  {21845, 1, 175},
  {21845, 0, 168},
  {21845, 0, 169},
  {21845, 0, 170},
  {21845, 0, 171},
  {21845, 0, 172},
  {21845, 0, 173},
  {21845, 0, 174},
  {21845, 0, 175},
  {21845, 0,  42},
  {21845, 0, 106},
  {21845, 0, 170},
  {21845, 0, 234},
  {21845, 0, 298},
  {21845, 0, 362},
  {21845, 0, 426},
  {21845, 0, 490},
  {21845, 1,  42},
  {21845, 1, 106},
  {21845, 1, 170},
  {21845, 1, 234},
  {21845, 1, 298},
  {21845, 1, 362},
  {21845, 1, 426},
  {21845, 1, 490},
  {21845, 0,  42},
  {21845, 0, 106},
  {21845, 0, 170},
  {21845, 0, 234},
  {21845, 0, 298},
  {21845, 0, 362},
  {21845, 0, 426},
  {21845, 0, 490},
  {21846, 1,  42},
  {21846, 0,  42},
  {21846, 0, 106},
  {21846, 0, 170},
  {21846, 0, 234},
  {21846, 0, 298},
  {21846, 0, 362},
  {21846, 0, 426},
  {21846, 0, 490},
  {21845, 1,  42},
  {21846, 1, 106},
  {21846, 0,  42},
  {21846, 0, 106},
  {21846, 0, 170},
  {21846, 0, 234},
  {21846, 0, 298},
  {21846, 0, 362},
  {21846, 0, 426},
  {21846, 0, 490},
  {21845, 1, 106},
  {21846, 1, 170},
  {21846, 0,  42},
  {21846, 0, 106},
  {21846, 0, 170},
  {21846, 0, 234},
  {21846, 0, 298},
  {21846, 0, 362},
  {21846, 0, 426},
  {21846, 0, 490},
  {21845, 1, 170},
  {21846, 1, 234},
  {21846, 0,  42},
  {21846, 0, 106},
  {21846, 0, 170},
  {21846, 0, 234},
  {21846, 0, 298},
  {21846, 0, 362},
  {21846, 0, 426},
  {21846, 0, 490},
  {21845, 1, 234},
  {21846, 1, 298},
  {21846, 0,  42},
  {21846, 0, 106},
  {21846, 0, 170},
  {21846, 0, 234},
  {21846, 0, 298},
  {21846, 0, 362},
  {21846, 0, 426},
  {21846, 0, 490},
  {21845, 1, 298},
  {21846, 1, 362},
  {21846, 0,  42},
  {21846, 0, 106},
  {21846, 0, 170},
  {21846, 0, 234},
  {21846, 0, 298},
  {21846, 0, 362},
  {21846, 0, 426},
  {21846, 0, 490},
  {21845, 1, 362},
  {21846, 1, 426},
  {21846, 0,  42},
  {21846, 0, 106},
  {21846, 0, 170},
  {21846, 0, 234},
  {21846, 0, 298},
  {21846, 0, 362},
  {21846, 0, 426},
  {21846, 0, 490},
  {21845, 1, 426},
  {21846, 1, 490},
  {21846, 0,  42},
  {21846, 0, 106},
  {21846, 0, 170},
  {21846, 0, 234},
  {21846, 0, 298},
  {21846, 0, 362},
  {21846, 0, 426},
  {21846, 0, 490},
  {21845, 1, 490},
  {21845, 0,  42},
  {21845, 0, 106},
  {21845, 0, 170},
  {21845, 0, 234},
  {21845, 0, 298},
  {21845, 0, 362},
  {21845, 0, 426},
  {21845, 0, 490},
};

static const std::vector<OutputType> Results = {
  0,     0,     0,     0,     13099, 0,     0,     4729,  4729,  13099, 13099,
  5119,  5119,  4729,  5119,  0,     0,     0,     0,     0,     0,     0,
  0,     21845, 21845, 21845, 21845, 21845, 21845, 21845, 21845, 21845, 21845,
  21845, 21845, 21845, 21845, 21845, 21845, 21846, 21846, 21845, 21845, 21845,
  21845, 21845, 21845, 21845, 21845, 21846, 21845, 21846, 21845, 21845, 21845,
  21845, 21845, 21845, 21845, 21846, 21845, 21845, 21846, 21845, 21845, 21845,
  21845, 21845, 21845, 21846, 21845, 21845, 21845, 21846, 21845, 21845, 21845,
  21845, 21845, 21846, 21845, 21845, 21845, 21845, 21846, 21845, 21845, 21845,
  21845, 21846, 21845, 21845, 21845, 21845, 21845, 21846, 21845, 21845, 21845,
  21846, 21845, 21845, 21845, 21845, 21845, 21845, 21846, 21845, 21845, 21846,
  21845, 21845, 21845, 21845, 21845, 21845, 21845, 21846, 21845, 21845, 21845,
  21845, 21845, 21845, 21845, 21845, 21845, 0,     0,     21845, 0,     0,
  0,     0,     0,     21845, 21845, 21845, 21845, 21845, 21845, 21845, 21845,
  21845, 21845, 21845, 21845, 21845, 21845, 21845, 21845, 21846, 21846, 21845,
  21845, 21845, 21845, 21845, 21845, 21845, 21845, 21846, 21845, 21846, 21845,
  21845, 21845, 21845, 21845, 21845, 21845, 21846, 21845, 21845, 21846, 21845,
  21845, 21845, 21845, 21845, 21845, 21846, 21845, 21845, 21845, 21846, 21845,
  21845, 21845, 21845, 21845, 21846, 21845, 21845, 21845, 21845, 21846, 21845,
  21845, 21845, 21845, 21846, 21845, 21845, 21845, 21845, 21845, 21846, 21845,
  21845, 21845, 21846, 21845, 21845, 21845, 21845, 21845, 21845, 21846, 21845,
  21845, 21846, 21845, 21845, 21845, 21845, 21845, 21845, 21845, 21846, 21845,
  21845, 21845, 21845, 21845, 21845, 21845, 21845, 21845,
};
}   //namespace ram512

namespace ram4k
{
static const std::vector<InputType> Inputs = {
  {     0, 0,    0},
  {     0, 0,    0},
  {     0, 1,    0},
  {  1111, 0,    0},
  {  1111, 1, 1111},
  {  1111, 0,    0},
  {  3513, 0, 3513},
  {  3513, 1, 3513},
  {  3513, 0, 3513},
  {  3513, 0, 1111},
  {  4095, 0, 1111},
  {  4095, 1, 4095},
  {  4095, 0, 4095},
  {  4095, 0, 3513},
  {  4095, 0, 4095},
  {  4095, 0, 2728},
  {  4095, 0, 2729},
  {  4095, 0, 2730},
  {  4095, 0, 2731},
  {  4095, 0, 2732},
  {  4095, 0, 2733},
  {  4095, 0, 2734},
  {  4095, 0, 2735},
  { 21845, 1, 2728},
  { 21845, 1, 2729},
  { 21845, 1, 2730},
  { 21845, 1, 2731},
  { 21845, 1, 2732},
  { 21845, 1, 2733},
  { 21845, 1, 2734},
  { 21845, 1, 2735},
  { 21845, 0, 2728},
  { 21845, 0, 2729},
  { 21845, 0, 2730},
  { 21845, 0, 2731},
  { 21845, 0, 2732},
  { 21845, 0, 2733},
  { 21845, 0, 2734},
  { 21845, 0, 2735},
  {-21846, 1, 2728},
  {-21846, 0, 2728},
  {-21846, 0, 2729},
  {-21846, 0, 2730},
  {-21846, 0, 2731},
  {-21846, 0, 2732},
  {-21846, 0, 2733},
  {-21846, 0, 2734},
  {-21846, 0, 2735},
  { 21845, 1, 2728},
  {-21846, 1, 2729},
  {-21846, 0, 2728},
  {-21846, 0, 2729},
  {-21846, 0, 2730},
  {-21846, 0, 2731},
  {-21846, 0, 2732},
  {-21846, 0, 2733},
  {-21846, 0, 2734},
  {-21846, 0, 2735},
  { 21845, 1, 2729},
  {-21846, 1, 2730},
  {-21846, 0, 2728},
  {-21846, 0, 2729},
  {-21846, 0, 2730},
  {-21846, 0, 2731},
  {-21846, 0, 2732},
  {-21846, 0, 2733},
  {-21846, 0, 2734},
  {-21846, 0, 2735},
  { 21845, 1, 2730},
  {-21846, 1, 2731},
  {-21846, 0, 2728},
  {-21846, 0, 2729},
  {-21846, 0, 2730},
  {-21846, 0, 2731},
  {-21846, 0, 2732},
  {-21846, 0, 2733},
  {-21846, 0, 2734},
  {-21846, 0, 2735},
  { 21845, 1, 2731},
  {-21846, 1, 2732},
  {-21846, 0, 2728},
  {-21846, 0, 2729},
  {-21846, 0, 2730},
  {-21846, 0, 2731},
  {-21846, 0, 2732},
  {-21846, 0, 2733},
  {-21846, 0, 2734},
  {-21846, 0, 2735},
  { 21845, 1, 2732},
  {-21846, 1, 2733},
  {-21846, 0, 2728},
  {-21846, 0, 2729},
  {-21846, 0, 2730},
  {-21846, 0, 2731},
  {-21846, 0, 2732},
  {-21846, 0, 2733},
  {-21846, 0, 2734},
  {-21846, 0, 2735},
  { 21845, 1, 2733},
  {-21846, 1, 2734},
  {-21846, 0, 2728},
  {-21846, 0, 2729},
  {-21846, 0, 2730},
  {-21846, 0, 2731},
  {-21846, 0, 2732},
  {-21846, 0, 2733},
  {-21846, 0, 2734},
  {-21846, 0, 2735},
  { 21845, 1, 2734},
  {-21846, 1, 2735},
  {-21846, 0, 2728},
  {-21846, 0, 2729},
  {-21846, 0, 2730},
  {-21846, 0, 2731},
  {-21846, 0, 2732},
  {-21846, 0, 2733},
  {-21846, 0, 2734},
  {-21846, 0, 2735},
  { 21845, 1, 2735},
  { 21845, 0, 2728},
  { 21845, 0, 2729},
  { 21845, 0, 2730},
  { 21845, 0, 2731},
  { 21845, 0, 2732},
  { 21845, 0, 2733},
  { 21845, 0, 2734},
  { 21845, 0, 2735},
  { 21845, 0,  341},
  { 21845, 0,  853},
  { 21845, 0, 1365},
  { 21845, 0, 1877},
  { 21845, 0, 2389},
  { 21845, 0, 2901},
  { 21845, 0, 3413},
  { 21845, 0, 3925},
  { 21845, 1,  341},
  { 21845, 1,  853},
  { 21845, 1, 1365},
  { 21845, 1, 1877},
  { 21845, 1, 2389},
  { 21845, 1, 2901},
  { 21845, 1, 3413},
  { 21845, 1, 3925},
  { 21845, 0,  341},
  { 21845, 0,  853},
  { 21845, 0, 1365},
  { 21845, 0, 1877},
  { 21845, 0, 2389},
  { 21845, 0, 2901},
  { 21845, 0, 3413},
  { 21845, 0, 3925},
  {-21846, 1,  341},
  {-21846, 0,  341},
  {-21846, 0,  853},
  {-21846, 0, 1365},
  {-21846, 0, 1877},
  {-21846, 0, 2389},
  {-21846, 0, 2901},
  {-21846, 0, 3413},
  {-21846, 0, 3925},
  { 21845, 1,  341},
  {-21846, 1,  853},
  {-21846, 0,  341},
  {-21846, 0,  853},
  {-21846, 0, 1365},
  {-21846, 0, 1877},
  {-21846, 0, 2389},
  {-21846, 0, 2901},
  {-21846, 0, 3413},
  {-21846, 0, 3925},
  { 21845, 1,  853},
  {-21846, 1, 1365},
  {-21846, 0,  341},
  {-21846, 0,  853},
  {-21846, 0, 1365},
  {-21846, 0, 1877},
  {-21846, 0, 2389},
  {-21846, 0, 2901},
  {-21846, 0, 3413},
  {-21846, 0, 3925},
  { 21845, 1, 1365},
  {-21846, 1, 1877},
  {-21846, 0,  341},
  {-21846, 0,  853},
  {-21846, 0, 1365},
  {-21846, 0, 1877},
  {-21846, 0, 2389},
  {-21846, 0, 2901},
  {-21846, 0, 3413},
  {-21846, 0, 3925},
  { 21845, 1, 1877},
  {-21846, 1, 2389},
  {-21846, 0,  341},
  {-21846, 0,  853},
  {-21846, 0, 1365},
  {-21846, 0, 1877},
  {-21846, 0, 2389},
  {-21846, 0, 2901},
  {-21846, 0, 3413},
  {-21846, 0, 3925},
  { 21845, 1, 2389},
  {-21846, 1, 2901},
  {-21846, 0,  341},
  {-21846, 0,  853},
  {-21846, 0, 1365},
  {-21846, 0, 1877},
  {-21846, 0, 2389},
  {-21846, 0, 2901},
  {-21846, 0, 3413},
  {-21846, 0, 3925},
  { 21845, 1, 2901},
  {-21846, 1, 3413},
  {-21846, 0,  341},
  {-21846, 0,  853},
  {-21846, 0, 1365},
  {-21846, 0, 1877},
  {-21846, 0, 2389},
  {-21846, 0, 2901},
  {-21846, 0, 3413},
  {-21846, 0, 3925},
  { 21845, 1, 3413},
  {-21846, 1, 3925},
  {-21846, 0,  341},
  {-21846, 0,  853},
  {-21846, 0, 1365},
  {-21846, 0, 1877},
  {-21846, 0, 2389},
  {-21846, 0, 2901},
  {-21846, 0, 3413},
  {-21846, 0, 3925},
  { 21845, 1, 3925},
  { 21845, 0,  341},
  { 21845, 0,  853},
  { 21845, 0, 1365},
  { 21845, 0, 1877},
  { 21845, 0, 2389},
  { 21845, 0, 2901},
  { 21845, 0, 3413},
  { 21845, 0, 3925},
};

static const std::vector<OutputType> Results = {
  0,      0,      0,      0,      1111,   0,      0,      3513,   3513,
  1111,   1111,   4095,   4095,   3513,   4095,   0,      0,      0,
  0,      0,      0,      0,      0,      21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  21845,  21845,  21845,  -21846, -21846, 21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  -21846, 21845,  -21846, 21845,  21845,
  21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  -21846,
  21845,  21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,
  21845,  -21846, 21845,  21845,  21845,  21845,  21845,  -21846, 21845,
  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  -21846,
  21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  21845,
  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  -21846, 21845,
  21845,  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  21845,
  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  21845,  0,      0,      0,      0,      0,      0,      0,      0,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  -21846, -21846,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  -21846,
  21845,  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  21845,
  -21846, 21845,  21845,  -21846, 21845,  21845,  21845,  21845,  21845,
  21845,  -21846, 21845,  21845,  21845,  -21846, 21845,  21845,  21845,
  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  -21846, 21845,
  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  21845,
  -21846, 21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,
  21845,  21845,  -21846, 21845,  21845,  -21846, 21845,  21845,  21845,
  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  21845,
};
}   //namespace ram4k

namespace ram16k
{
static const std::vector<InputType> Inputs = {
  {     0, 0,     0},
  {     0, 0,     0},
  {     0, 1,     0},
  {  4321, 0,     0},
  {  4321, 1,  4321},
  {  4321, 0,     0},
  { 12345, 0, 12345},
  { 12345, 1, 12345},
  { 12345, 0, 12345},
  { 12345, 0,  4321},
  { 16383, 0,  4321},
  { 16383, 1, 16383},
  { 16383, 0, 16383},
  { 16383, 0, 12345},
  { 16383, 0, 16383},
  { 16383, 0, 10920},
  { 16383, 0, 10921},
  { 16383, 0, 10922},
  { 16383, 0, 10923},
  { 16383, 0, 10924},
  { 16383, 0, 10925},
  { 16383, 0, 10926},
  { 16383, 0, 10927},
  { 21845, 1, 10920},
  { 21845, 1, 10921},
  { 21845, 1, 10922},
  { 21845, 1, 10923},
  { 21845, 1, 10924},
  { 21845, 1, 10925},
  { 21845, 1, 10926},
  { 21845, 1, 10927},
  { 21845, 0, 10920},
  { 21845, 0, 10921},
  { 21845, 0, 10922},
  { 21845, 0, 10923},
  { 21845, 0, 10924},
  { 21845, 0, 10925},
  { 21845, 0, 10926},
  { 21845, 0, 10927},
  {-21846, 1, 10920},
  {-21846, 0, 10920},
  {-21846, 0, 10921},
  {-21846, 0, 10922},
  {-21846, 0, 10923},
  {-21846, 0, 10924},
  {-21846, 0, 10925},
  {-21846, 0, 10926},
  {-21846, 0, 10927},
  { 21845, 1, 10920},
  {-21846, 1, 10921},
  {-21846, 0, 10920},
  {-21846, 0, 10921},
  {-21846, 0, 10922},
  {-21846, 0, 10923},
  {-21846, 0, 10924},
  {-21846, 0, 10925},
  {-21846, 0, 10926},
  {-21846, 0, 10927},
  { 21845, 1, 10921},
  {-21846, 1, 10922},
  {-21846, 0, 10920},
  {-21846, 0, 10921},
  {-21846, 0, 10922},
  {-21846, 0, 10923},
  {-21846, 0, 10924},
  {-21846, 0, 10925},
  {-21846, 0, 10926},
  {-21846, 0, 10927},
  { 21845, 1, 10922},
  {-21846, 1, 10923},
  {-21846, 0, 10920},
  {-21846, 0, 10921},
  {-21846, 0, 10922},
  {-21846, 0, 10923},
  {-21846, 0, 10924},
  {-21846, 0, 10925},
  {-21846, 0, 10926},
  {-21846, 0, 10927},
  { 21845, 1, 10923},
  {-21846, 1, 10924},
  {-21846, 0, 10920},
  {-21846, 0, 10921},
  {-21846, 0, 10922},
  {-21846, 0, 10923},
  {-21846, 0, 10924},
  {-21846, 0, 10925},
  {-21846, 0, 10926},
  {-21846, 0, 10927},
  { 21845, 1, 10924},
  {-21846, 1, 10925},
  {-21846, 0, 10920},
  {-21846, 0, 10921},
  {-21846, 0, 10922},
  {-21846, 0, 10923},
  {-21846, 0, 10924},
  {-21846, 0, 10925},
  {-21846, 0, 10926},
  {-21846, 0, 10927},
  { 21845, 1, 10925},
  {-21846, 1, 10926},
  {-21846, 0, 10920},
  {-21846, 0, 10921},
  {-21846, 0, 10922},
  {-21846, 0, 10923},
  {-21846, 0, 10924},
  {-21846, 0, 10925},
  {-21846, 0, 10926},
  {-21846, 0, 10927},
  { 21845, 1, 10926},
  {-21846, 1, 10927},
  {-21846, 0, 10920},
  {-21846, 0, 10921},
  {-21846, 0, 10922},
  {-21846, 0, 10923},
  {-21846, 0, 10924},
  {-21846, 0, 10925},
  {-21846, 0, 10926},
  {-21846, 0, 10927},
  { 21845, 1, 10927},
  { 21845, 0, 10920},
  { 21845, 0, 10921},
  { 21845, 0, 10922},
  { 21845, 0, 10923},
  { 21845, 0, 10924},
  { 21845, 0, 10925},
  { 21845, 0, 10926},
  { 21845, 0, 10927},
  { 21845, 0,  1365},
  { 21845, 0,  3413},
  { 21845, 0,  5461},
  { 21845, 0,  7509},
  { 21845, 0,  9557},
  { 21845, 0, 11605},
  { 21845, 0, 13653},
  { 21845, 0, 15701},
  { 21845, 1,  1365},
  { 21845, 1,  3413},
  { 21845, 1,  5461},
  { 21845, 1,  7509},
  { 21845, 1,  9557},
  { 21845, 1, 11605},
  { 21845, 1, 13653},
  { 21845, 1, 15701},
  { 21845, 0,  1365},
  { 21845, 0,  3413},
  { 21845, 0,  5461},
  { 21845, 0,  7509},
  { 21845, 0,  9557},
  { 21845, 0, 11605},
  { 21845, 0, 13653},
  { 21845, 0, 15701},
  {-21846, 1,  1365},
  {-21846, 0,  1365},
  {-21846, 0,  3413},
  {-21846, 0,  5461},
  {-21846, 0,  7509},
  {-21846, 0,  9557},
  {-21846, 0, 11605},
  {-21846, 0, 13653},
  {-21846, 0, 15701},
  { 21845, 1,  1365},
  {-21846, 1,  3413},
  {-21846, 0,  1365},
  {-21846, 0,  3413},
  {-21846, 0,  5461},
  {-21846, 0,  7509},
  {-21846, 0,  9557},
  {-21846, 0, 11605},
  {-21846, 0, 13653},
  {-21846, 0, 15701},
  { 21845, 1,  3413},
  {-21846, 1,  5461},
  {-21846, 0,  1365},
  {-21846, 0,  3413},
  {-21846, 0,  5461},
  {-21846, 0,  7509},
  {-21846, 0,  9557},
  {-21846, 0, 11605},
  {-21846, 0, 13653},
  {-21846, 0, 15701},
  { 21845, 1,  5461},
  {-21846, 1,  7509},
  {-21846, 0,  1365},
  {-21846, 0,  3413},
  {-21846, 0,  5461},
  {-21846, 0,  7509},
  {-21846, 0,  9557},
  {-21846, 0, 11605},
  {-21846, 0, 13653},
  {-21846, 0, 15701},
  { 21845, 1,  7509},
  {-21846, 1,  9557},
  {-21846, 0,  1365},
  {-21846, 0,  3413},
  {-21846, 0,  5461},
  {-21846, 0,  7509},
  {-21846, 0,  9557},
  {-21846, 0, 11605},
  {-21846, 0, 13653},
  {-21846, 0, 15701},
  { 21845, 1,  9557},
  {-21846, 1, 11605},
  {-21846, 0,  1365},
  {-21846, 0,  3413},
  {-21846, 0,  5461},
  {-21846, 0,  7509},
  {-21846, 0,  9557},
  {-21846, 0, 11605},
  {-21846, 0, 13653},
  {-21846, 0, 15701},
  { 21845, 1, 11605},
  {-21846, 1, 13653},
  {-21846, 0,  1365},
  {-21846, 0,  3413},
  {-21846, 0,  5461},
  {-21846, 0,  7509},
  {-21846, 0,  9557},
  {-21846, 0, 11605},
  {-21846, 0, 13653},
  {-21846, 0, 15701},
  { 21845, 1, 13653},
  {-21846, 1, 15701},
  {-21846, 0,  1365},
  {-21846, 0,  3413},
  {-21846, 0,  5461},
  {-21846, 0,  7509},
  {-21846, 0,  9557},
  {-21846, 0, 11605},
  {-21846, 0, 13653},
  {-21846, 0, 15701},
  { 21845, 1, 15701},
  { 21845, 0,  1365},
  { 21845, 0,  3413},
  { 21845, 0,  5461},
  { 21845, 0,  7509},
  { 21845, 0,  9557},
  { 21845, 0, 11605},
  { 21845, 0, 13653},
  { 21845, 0, 15701},
};
static const std::vector<OutputType> Results = {
  0,
  0,      0,      0,      4321,   0,      0,      12345,  12345,  4321,
  4321,   16383,  16383,  12345,  16383,  0,      0,      0,      0,
  0,      0,      0,      0,      21845,  21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  21845,  21845,  -21846, -21846, 21845,  21845,  21845,  21845,  21845,
  21845,  21845,  21845,  -21846, 21845,  -21846, 21845,  21845,  21845,
  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  -21846, 21845,
  21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  21845,
  -21846, 21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,
  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  -21846, 21845,
  21845,  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  -21846,
  21845,  21845,  21845,  21845,  21845,  21845,  -21846, 21845,  21845,
  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  21845,  -21846,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  0,      0,      0,      0,      0,      0,      0,      0,      21845,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,  21845,  21845,  -21846, -21846, 21845,
  21845,  21845,  21845,  21845,  21845,  21845,  21845,  -21846, 21845,
  -21846, 21845,  21845,  21845,  21845,  21845,  21845,  21845,  -21846,
  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  21845,  21845,
  -21846, 21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,
  21845,  -21846, 21845,  21845,  21845,  21845,  -21846, 21845,  21845,
  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  21845,  -21846,
  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  21845,
  21845,  -21846, 21845,  21845,  -21846, 21845,  21845,  21845,  21845,
  21845,  21845,  21845,  -21846, 21845,  21845,  21845,  21845,  21845,
  21845,  21845,  21845,  21845,
};

}   //namespace ram16k

namespace ram32k
{
static const std::vector<InputType>  Inputs  = {};
static const std::vector<OutputType> Results = {};
}   //namespace ram32k

}   //namespace test::memory
#endif