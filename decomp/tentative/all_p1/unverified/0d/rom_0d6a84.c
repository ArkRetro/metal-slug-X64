/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6a84.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6a84(void)

{
  byte bVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_1ad6e();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0x10;
  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  candidate_4256();
  *unaff_A6 = 0xd6ac6;
  uVar2 = 0;
  candidate_d7198();
  candidate_427a();
  if ((bool)uVar2) {
    candidate_120f8();
    *unaff_A6 = candidate_d6b10;
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0x7a);
  if (bVar1 == 5) {
    *unaff_A6 = candidate_d6c66;
  }
  bVar3 = bVar1 < 5;
  candidate_19504();
  if (bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
