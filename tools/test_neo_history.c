#include <assert.h>
#include <stdio.h>
#include "../src/neo_history.h"
#include "history_fixture.h"
int main(void) {
    for (unsigned i=0;i<sizeof(history_cases)/sizeof(history_cases[0]);i++)
        assert(neo_history_token_next(history_cases[i][0])==history_cases[i][1]);
    for (unsigned i=0;i<256;i++) {
        uint8_t next=neo_history_token_next((uint8_t)i);assert(next==1 || next==2);
        assert(neo_history_token_next(next)!=next);
    }
    puts("PASS: 180 original history-token writes; all byte inputs advance to nonzero 1/2");
}
