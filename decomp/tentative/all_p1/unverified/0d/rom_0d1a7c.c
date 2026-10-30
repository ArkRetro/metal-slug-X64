/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1a7c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d1a7c(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_4256();
  *unaff_A6 = 0xd1a9c;
  candidate_d1b6c();
  candidate_d1b28();
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -1;
  uVar1 = *(ushort *)(unaff_A6 + 0xb);
  if (uVar1 == 8) {
    *(undefined1 *)(unaff_A6[3] + 0x60) = 4;
    *unaff_A6 = 0xd19b4;
  }
  bVar2 = uVar1 < 8;
  candidate_19424();
  if (bVar2) {
    *unaff_A6 = candidate_d1ac6;
  }
  return;
}
