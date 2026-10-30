/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09cffe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9cffe(void)

{
  ushort extraout_D1w;
  short sVar1;
  short sVar2;
  undefined4 extraout_A0;
  undefined4 extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar3;
  int extraout_A0_03;
  int extraout_A1;
  int extraout_A1_00;
  int unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  *(char *)(unaff_A6 + 0x89) = *(char *)(unaff_A6 + 0x89) + '\x01';
  if (*(char *)(unaff_A6 + 0x89) < '\x0f') {
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x89) = 0xf;
  bVar4 = false;
  candidate_1aeb0();
  if (bVar4) {
    *(undefined4 *)(unaff_A6 + 0x76) = extraout_A0;
    uVar5 = false;
    candidate_1aeb0();
    if ((bool)uVar5) {
      candidate_1b08a();
      if ((bool)uVar5) {
        *(undefined4 *)(unaff_A6 + 0x76) = extraout_A0_00;
        bVar4 = false;
        candidate_1b08a();
        iVar3 = extraout_A0_02;
        if (bVar4) {
          return;
        }
      }
      else {
        *(undefined4 *)(unaff_A6 + 0x76) = extraout_A0_00;
        bVar4 = false;
        candidate_1b08a();
        iVar3 = extraout_A0_01;
        if (!bVar4) {
          return;
        }
      }
      bVar4 = *(ushort *)(iVar3 + 0x40) < *(ushort *)(unaff_A6 + 0x40);
      candidate_9d124();
      if (bVar4) {
        bVar4 = *(ushort *)(extraout_A1 + 0x40) < extraout_D1w;
        candidate_9d124();
        if (bVar4) {
          sVar1 = *(short *)(extraout_A0_03 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
          sVar2 = *(short *)(extraout_A1_00 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
          if (sVar1 < 0) {
            sVar1 = -sVar1;
          }
          if (sVar2 < 0) {
            sVar2 = -sVar2;
          }
          if ((sVar1 < 0x21) && (sVar2 < 0x21)) {
            return;
          }
        }
      }
      else {
        bVar4 = *(ushort *)(extraout_A1 + 0x40) < extraout_D1w;
        candidate_9d124();
        if (!bVar4) {
          return;
        }
      }
      candidate_1b08a();
      return;
    }
  }
  else {
    uVar5 = false;
    candidate_1aeb0();
    if (!(bool)uVar5) {
      return;
    }
  }
  candidate_1b08a();
  if (!(bool)uVar5) {
    return;
  }
  return;
}
