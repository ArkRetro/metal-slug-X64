/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c6f34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c6f34(void)

{
  short sVar1;
  short in_D0w;
  short in_D1w;
  int in_A0;
  undefined4 in_A1;
  int unaff_A2;

  *(undefined4 *)(in_A0 + 0x2e) = in_A1;
  sVar1 = (short)(in_D1w << 4) >> 4;
  *(short *)(in_A0 + 0x32) = (short)(in_D0w << 4) >> 4;
  *(short *)(in_A0 + 0x34) = sVar1;
  if (unaff_A2 == -1) {
    *(char *)(in_A0 + 0x83) = *(char *)(in_A0 + 0x34) - (char)sVar1;
  }
  else {
    *(undefined1 *)(in_A0 + 0x83) = 0;
  }
  *(int *)(in_A0 + 0x3e) = unaff_A2;
  return;
}
