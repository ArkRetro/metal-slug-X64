/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x21ca; tentative_c. */

char z80_candidate_21ca(char *param_1)

{
  if (*param_1 == '\0') {
    return *param_1;
  }
  *param_1 = '\x04';
  param_1[10] = param_1[8];
  param_1[0xb] = param_1[9];
  return param_1[9];
}
