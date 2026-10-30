/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x048f8c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_48f8c(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  short sVar4;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_1abc8();
  sVar4 = *(short *)(unaff_A6 + 0x25) + -1;
  *(short *)(unaff_A6 + 0x25) = sVar4;
  if (sVar4 != 0) {
    return;
  }
  *(undefined2 *)(unaff_A6 + 0x25) = *(undefined2 *)((int)unaff_A6 + 0x8e);
  unaff_A6[10] = unaff_A6[0xc] + unaff_A6[10];
  uVar3 = unaff_A6[0xb];
  bVar5 = CARRY4(unaff_A6[0xd],uVar3);
  unaff_A6[0xb] = unaff_A6[0xd] + uVar3;
  candidate_1b1ec();
  if (!bVar5) {
    candidate_40bc();
    candidate_1acf2();
    *(undefined2 *)(extraout_A0 + 0x66) = 0x1001;
    *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
    sVar4 = *(short *)(unaff_A6 + 0x23);
    *(undefined1 *)(extraout_A0 + 0x12) = *(undefined1 *)((short)(sVar4 + -1) + 0x49006);
    *(undefined1 *)(extraout_A0 + 0x13) = *(undefined1 *)((short)(sVar4 + -1) + 0x49006);
    sVar4 = *(short *)(unaff_A6 + 0x23) + -1;
    *(short *)(unaff_A6 + 0x23) = sVar4;
    if (sVar4 != 0) {
      return;
    }
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
