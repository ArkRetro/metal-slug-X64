/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06cf9c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6cf9c(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  short sVar4;
  int extraout_A0;
  undefined4 extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar5;

  candidate_6d384();
  if ((bool)in_CF) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0x14;
    *(byte *)(unaff_A6 + 0x20) = *(byte *)(unaff_A6 + 0x20) | 2;
    candidate_6d238();
    *unaff_A6 = 0x6cfdc;
    if (*(int *)((int)unaff_A6 + 0x6e) == 0) {
      if (*(short *)(unaff_A6 + 0x1f) == 0) {
        *(undefined2 *)(unaff_A6 + 0x1f) = 0x14;
        uVar3 = candidate_29c6();
                    /* WARNING: Could not recover jumptable at 0x0006cefc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*(code *)(extraout_A0 + (short)((uVar3 & 3) * 4)))();
        return;
      }
    }
    else if (-1 < (char)(-((*(byte *)(unaff_A6 + 0x20) & 1) == 0) ^
                        -((short)(*(short *)(*(int *)((int)unaff_A6 + 0x6e) + 0x3c) -
                                 *(short *)(unaff_A6 + 0xf)) < 0))) {
      candidate_6d282();
    }
    candidate_6d088();
    candidate_6d17c();
  }
  else {
    candidate_1436a();
    candidate_427a();
    if ((bool)in_CF) {
      *unaff_A6 = candidate_6cec4;
    }
  }
  sVar4 = candidate_8c0c2();
  if (sVar4 < 0) {
    *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
    bVar5 = false;
    candidate_1affc();
    if (!bVar5) {
      *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A0_00;
    }
    bVar5 = false;
    if (*(short *)(unaff_A6 + 0x1f) != 0) {
      sVar4 = *(short *)(unaff_A6 + 0x1f);
      bVar5 = sVar4 == 0;
      *(short *)(unaff_A6 + 0x1f) = sVar4 + -1;
    }
    candidate_1945c();
    if (bVar5) {
      *unaff_A6 = 0x6d022;
    }
    return;
  }
  candidate_8ce42();
  candidate_4192();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
