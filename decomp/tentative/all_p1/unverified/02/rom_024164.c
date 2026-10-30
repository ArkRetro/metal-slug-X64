/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024164.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24164(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 uVar3;
  byte *pbVar4;
  short sVar5;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_120f8();
  candidate_40bc();
  candidate_1acf2();
  unaff_A6[0x14] = 0;
  candidate_4256();
  if ((*(byte *)(unaff_A6[3] + 0x8f) & '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7)) == 0) {
    pbVar4 = (byte *)(unaff_A6[3] + 0x8f);
    *pbVar4 = *pbVar4 | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
  }
  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  *unaff_A6 = 0x241cc;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x24) == 3) {
    sVar5 = *(short *)(unaff_A6[3] + 0x9a) * 4;
    if (*(short *)(unaff_A6[3] + 0x9a) == 9) {
      sVar5 = sVar5 + -1;
    }
    uVar3 = unaff_A6[0xf];
    *(short *)(unaff_A6 + 0xf) = sVar5 + *(short *)(unaff_A6 + 0xf);
    candidate_427a();
    unaff_A6[0xf] = uVar3;
  }
  else {
    candidate_427a();
  }
  if ((*(byte *)(unaff_A6[3] + 0x8d) & 4) != 0) {
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
