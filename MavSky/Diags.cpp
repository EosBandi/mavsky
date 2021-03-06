//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  A copy of the GNU General Public License is available at <http://www.gnu.org/licenses/>.
//  
#include "Diags.h"

Diags::Diags() {
}

void Diags::set_fault_to(uint8_t fault_id, uint8_t fault_value) {
  faults[fault_id] = fault_value;
}

void Diags::clear_fault(uint8_t fault_id) {
  faults[fault_id] = 0;
}

void Diags::clear_all_faults() {
  for(int i=0; i<MAX_FAULTS; i++) {
      faults[i] = 0;
  }
}

void Diags::load_new_faults() {
  current_flash_code_count = 0;
  for(int i=0; i<MAX_FAULTS; i++) {
    if(faults[i]) {
      current_flash_codes[current_flash_code_count++] = i;
    }
  }
}

void Diags::update_led() {
    switch(next_flash_state) {
      case 0:
        if(current_flash_code_count > 0) {
          if(current_code_index >= current_flash_code_count) {
            current_code_index = 0;
            load_new_faults();
          }
          flashes_remaining = current_flash_codes[current_code_index] - 1;
          next_flash_state = 1;
        } else {
            load_new_faults();
        }
        digitalWrite(LEDPIN, HIGH);  
        break;
      case 1: 
        digitalWrite(LEDPIN, LOW);
        if(flashes_remaining > 0) {
           next_flash_state = 2;
        } else {
           next_flash_state = 3;
        }
        break;
      case 2:
        digitalWrite(LEDPIN, HIGH);   
        flashes_remaining--;
        next_flash_state = 1;
        break;
      case 3:
        digitalWrite(LEDPIN, LOW);  
        next_flash_state = 4;
        break;
      case 4:
        digitalWrite(LEDPIN, LOW);  
        next_flash_state = 5;
        break;
      case 5:
        digitalWrite(LEDPIN, LOW);  
        next_flash_state = 0;
        current_code_index++;
        break;
    } 
}
