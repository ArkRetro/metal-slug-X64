/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09edf0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9edf0(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x9ee1a;
  uVar2 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_9ec34;
  }
  bVar1 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
