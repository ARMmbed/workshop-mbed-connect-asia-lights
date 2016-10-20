/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "select_project.h"

#define BUILT_IN_LED_OFF        1
#define BUILT_IN_LED_ON         0

#if PROJECT == 1
#include "1_movement/main.h"
#elif PROJECT == 2
#include "2_connected/main.h"
#else
#error "Select a project (between 1 and 2) in select_project.h first!"
#endif
