# Copyright 2018 bmc::labs GmbH.
# Author: Florian Eich <florian@bmc-labs.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# this is just a super simple showoff executable running the code in the README
set (TARGET progress)
add_executable (${TARGET} main.cc)
target_include_directories (${TARGET} PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)

