/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0401ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_401ae(void)

{
  uint uVar1;
  char in_D0b;
  uint in_A0;

  uVar1 = in_A0 + (int)*(short *)(in_A0 + 0x50);
  while( true ) {
    if (uVar1 <= in_A0) {
      return;
    }
    if (in_D0b == *(char *)(in_A0 + 9)) break;
    in_A0 = in_A0 + 10;
  }
  return;
}
