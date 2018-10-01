#ifndef ERROR_H
#define ERROR_H

void raise_manual_trigger_error(int group);
void raise_illegal_mode(int mode);
void raise_zero_period_trigger(int group_idx, int line);
void raise_read_request_not_in_range(int group_idx, int line);

#endif // ERROR_H
