/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f028.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f028(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined8 uVar2;

  sVar1 = candidate_29f2();
  *(short *)(unaff_A6 + 0x20) = sVar1 + 0x300;
  uVar2 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar2;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  candidate_4256();
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  *unaff_A6 = candidate_3f084;
  candidate_3fada();
  return;
}
