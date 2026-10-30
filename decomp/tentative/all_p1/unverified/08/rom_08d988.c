/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d988.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_8d988(void)

{
  short sVar1;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar2;

  bVar2 = false;
  if ((_DAT_0010000e & 0x1f) == 0) {
    candidate_8d9d0();
    if (bVar2) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    }
    *(undefined4 *)((int)unaff_A6 + 0x7e) = extraout_A0;
  }
  bVar2 = false;
  candidate_8d940();
  if (bVar2) {
    *unaff_A6 = candidate_8dbe8;
  }
  sVar1 = candidate_510c();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_b05fe;
  }
  return;
}
