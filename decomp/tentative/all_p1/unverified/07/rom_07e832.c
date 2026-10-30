/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07e832.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7e832(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  unaff_A6[0x1f] = (int)unaff_A6[0x1f] >> 1;
  candidate_7ef24();
  candidate_4256();
  *unaff_A6 = 0x7e856;
  func_0x000143e4();
  unaff_A6[0xc] = unaff_A6[0x1e] + unaff_A6[0xc];
  uVar3 = unaff_A6[0xd];
  uVar4 = CARRY4(unaff_A6[0x1f],uVar3);
  unaff_A6[0xd] = unaff_A6[0x1f] + uVar3;
  candidate_427a();
  candidate_1affc();
  if (!(bool)uVar4) {
    candidate_1affc();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_7e902;
    }
  }
  uVar4 = false;
  candidate_1495c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_7e902;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_7ea1e;
    uVar4 = 0;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    candidate_40bc();
    candidate_1acf2();
    *unaff_A6 = candidate_7ea6c;
    uVar4 = 0;
  }
  candidate_1945c();
  if ((bool)uVar4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
