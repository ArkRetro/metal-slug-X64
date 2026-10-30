/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0338b8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_338b8(void)

{
  code *pcVar1;
  short sVar2;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  sVar2 = *(short *)(unaff_A6 + 0x23) + -1;
  *(short *)(unaff_A6 + 0x23) = sVar2;
  if (sVar2 == 0) {
    candidate_4126();
    iVar3 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar3 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    iVar3 = unaff_A6[0xd];
    if (iVar3 < 0) {
      iVar3 = -iVar3;
    }
    unaff_A6[0xd] = iVar3 * 3 >> 2;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
    candidate_4256();
  }
  candidate_427a();
  return;
}
