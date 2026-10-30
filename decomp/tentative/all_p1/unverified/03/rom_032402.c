/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x032402.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_32402(void)

{
  code *pcVar1;
  short sVar2;
  int iVar3;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  sVar2 = candidate_29f2();
  *(undefined1 *)(unaff_A6 + 0x1f) = *(undefined1 *)(sVar2 + 0x323fa);
  do {
    candidate_40bc();
    candidate_1acf2();
    while ((*(byte *)(unaff_A6 + 0x1f) & '\x01' << (*(ushort *)((int)unaff_A6 + 0x6e) & 7)) == 0) {
      *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 1;
    }
    iVar3 = (uint)(byte)*(ushort *)((int)unaff_A6 + 0x6e) * 0x10;
    *(undefined4 *)(extraout_A0 + 0x30) = *(undefined4 *)(iVar3 + 0x32368);
    *(undefined4 *)(extraout_A0 + 0x34) = *(undefined4 *)(iVar3 + 0x3236c);
    *(undefined4 *)(extraout_A0 + 0x74) = *(undefined4 *)(iVar3 + 0x32370);
    *(undefined4 *)(extraout_A0 + 0x78) = *(undefined4 *)(iVar3 + 0x32374);
    *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 1;
    sVar2 = *(short *)(unaff_A6 + 0x1c) + -1;
    *(short *)(unaff_A6 + 0x1c) = sVar2;
  } while (sVar2 != 0);
  *unaff_A6 = 0x3247a;
  if (*(short *)((int)unaff_A6 + 0x72) == 0) {
    candidate_4126();
    iVar3 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar3 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
