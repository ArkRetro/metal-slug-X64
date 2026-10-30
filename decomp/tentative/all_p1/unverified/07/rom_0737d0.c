/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0737d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_737d0(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_49e8();
  candidate_ae220();
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0 + 0x30) = 0xfff20000;
  *(undefined4 *)(extraout_A0 + 0x34) = 0x280000;
  *(undefined2 *)(extraout_A0 + 0x4e) = 10;
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0_00 + 0x30) = 0xa0000;
  *(undefined4 *)(extraout_A0_00 + 0x34) = 0xffdb0000;
  *(undefined2 *)(extraout_A0_00 + 0x4e) = 7;
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0_01 + 0x30) = 0xfff70000;
  *(undefined4 *)(extraout_A0_01 + 0x34) = 0x10000;
  *(undefined2 *)(extraout_A0_01 + 0x4e) = 7;
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0_02 + 0x30) = 0x70000;
  *(undefined4 *)(extraout_A0_02 + 0x34) = 0xfffc0000;
  *(undefined2 *)(extraout_A0_02 + 0x4e) = 5;
  candidate_411c();
  candidate_1acf2();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
