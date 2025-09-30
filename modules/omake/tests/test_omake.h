/**************************************************************************/
/*  test_omake.h                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                            BLAZIUM ENGINE                              */
/*                          https://blazium.app                           */
/**************************************************************************/
/* Copyright (c) 2024-present Blazium Engine contributors.                */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "tests/test_macros.h"

#include "modules/omake/omake.h"

namespace TestOmakeMisc {

TEST_CASE("[Omake] add_clampedi") {
	CHECK(6 == Omake::add_clampedi(1, 5, 0, 100)); // Normal add
	CHECK(-4 == Omake::add_clampedi(1, -5, -100, 100)); // Normal add
	CHECK(0 == Omake::add_clampedi(6, -900, 0, 100)); // Clamp Underflow
	CHECK(100 == Omake::add_clampedi(6, 900, 0, 100)); // Clamp Overflow

	CHECK(INT64_MAX == Omake::add_clampedi(7, INT64_MAX)); // Clamp Overflow
	CHECK(INT64_MIN == Omake::add_clampedi(-8, INT64_MIN)); // Clamp Underflow
	CHECK(0 == Omake::add_clampedi(0, INT64_MIN, 0, INT64_MAX)); // Clamp Underflow
}

} //namespace TestOmakeMisc
