/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd200.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd200(void)

{
  byte bVar1;
  byte bVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x800;
  candidate_9ea8();
  candidate_4520();
  *(undefined1 *)((int)unaff_A6 + 0x73) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
  candidate_4256();
  *unaff_A6 = 0xcd268;
  candidate_ce6b2();
  candidate_427a();
  *(undefined1 *)((int)unaff_A6 + 0x87) = *(undefined1 *)(unaff_A6[3] + 0x87);
  bVar1 = *(byte *)(unaff_A6[3] + 0x73);
  bVar2 = *(byte *)((int)unaff_A6 + 0x73);
  if (bVar1 != bVar2) {
    *(byte *)((int)unaff_A6 + 0x73) = bVar1;
    *unaff_A6 = 0xcd238;
  }
  bVar3 = bVar1 == bVar2 && bVar1 < bVar2;
  candidate_409a();
  if (!bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
