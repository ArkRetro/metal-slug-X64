/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x084a02.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_84a02(void)

{
  undefined4 uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  uVar1 = unaff_A6[0xc];
  unaff_A6[0xd] = 0x2aaaa;
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xffff1c72;
  unaff_A6[0xc] = 0;
  unaff_A6[0xc] = uVar1;
  candidate_4256();
  *unaff_A6 = 0x84a50;
  *(undefined4 *)((int)unaff_A6 + 0x72) = unaff_A6[0xd];
  func_0x000143e4();
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x6e) + unaff_A6[0xd];
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_84a82;
  }
  candidate_427a();
  candidate_84b86();
  if (*(short *)((int)unaff_A6 + 0x76) < 1) {
    *unaff_A6 = candidate_41ac;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x2f) {
    *unaff_A6 = candidate_41ac;
  }
  bVar2 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
