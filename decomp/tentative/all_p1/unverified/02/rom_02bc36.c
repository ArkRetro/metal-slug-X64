/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02bc36.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2bc36(void)

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
  *unaff_A6 = 0x2bc84;
  *(undefined4 *)((int)unaff_A6 + 0x76) = unaff_A6[0xd];
  candidate_2bdd8();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_2bcac;
  }
  candidate_427a();
  candidate_2c18e();
  if (*(short *)((int)unaff_A6 + 0x82) < 1) {
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
