/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b96e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_b96e6(void)

{
  code *pcVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int unaff_A5;
  undefined4 *unaff_A6;

  iVar4 = (uint)*(ushort *)((int)unaff_A6 + 0x6e) * 0x10000 -
          *(int *)(candidate_698c + unaff_A5 + 6);
  if (iVar4 == 0) {
    candidate_4126();
    iVar4 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar4 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  iVar5 = unaff_A6[0x1c];
  if ((int)unaff_A6[0xb] < 0) {
    iVar5 = -iVar5;
  }
  else {
    iVar2 = unaff_A6[0xd];
    unaff_A6[0xd] = iVar5 + iVar2;
    unaff_A6[0xb] = unaff_A6[0xb] - (iVar5 + iVar2);
  }
  iVar2 = *(int *)(unaff_A5 + 0x6960);
  if (iVar4 < 0) {
    if (iVar2 < iVar5) {
      *(int *)(unaff_A5 + 0x6960) = iVar2 - iVar5;
    }
  }
  else {
    iVar3 = iVar5 + iVar2;
    if (iVar3 != 0 && SCARRY4(iVar5,iVar2) == iVar3 < 0) {
      *(int *)(unaff_A5 + 0x6960) = iVar3;
    }
  }
  return iVar4;
}
