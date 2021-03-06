/*
###################################################################################
#
# Cutlib - Cut Information Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

/// @file
/// @brief 時間測定用クラス 実装
///

#include "CutTiming.h"

namespace cutlib {

/// 時間測定ストップウオッチリスト.
Timer Timer::Timers[NumSections];

} // namespace cutlib
