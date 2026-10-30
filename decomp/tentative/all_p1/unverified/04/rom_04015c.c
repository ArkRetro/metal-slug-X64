/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04015c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4015c(void)

{
  int *piVar1;
  int in_D0;
  int in_D1;
  uint uVar2;
  uint uVar3;
  int *in_A0;

  uVar3 = 0xffffffff;
  piVar1 = (int *)((int)in_A0 + (int)*(short *)(in_A0 + 0x14));
  for (; in_A0 < piVar1; in_A0 = (int *)((int)in_A0 + 10)) {
    if ((uint)(in_D1 - in_A0[1]) < (uint)*(byte *)(in_A0 + 2) << 0x10) {
      uVar2 = in_D0 - (*in_A0 - ((uint)(in_D1 - in_A0[1]) >> 3));
      if ((int)uVar2 < 0) {
        uVar2 = -uVar2;
      }
      if (uVar2 < uVar3) {
        uVar3 = uVar2;
      }
    }
  }
  return;
}
