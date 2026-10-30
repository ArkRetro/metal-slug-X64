/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0857de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_857de(void)

{
  short sVar1;
  undefined2 uVar2;
  ushort uVar3;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x1d) = uVar2;
  if ((*(byte *)((int)unaff_A6 + 0x85) & 1) != 0) {
    candidate_40bc();
    uVar3 = *(short *)(unaff_A6 + 0x1d) - 8;
    if ((short)uVar3 < 0) {
      uVar3 = 0;
    }
    *(char *)(extraout_A0 + 0x86) = (char)(uVar3 >> 1);
  }
  *unaff_A6 = 0x85820;
  sVar1 = *(short *)(unaff_A6 + 0x1d);
  uVar4 = sVar1 == 0;
  sVar1 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1d) = sVar1;
  if (sVar1 < 0) {
    uVar2 = 0xff80;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      uVar2 = 0x1c0;
    }
    *(undefined2 *)((int)unaff_A6 + 0x72) = uVar2;
    sVar1 = candidate_260c8();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      sVar1 = -sVar1;
    }
    unaff_A6[0xc] = (int)sVar1 << 8;
    unaff_A6[0xd] = 0;
    *unaff_A6 = candidate_857b2;
    candidate_1ac38();
    candidate_85850();
    if ((char)(-((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) ^
              -(*(short *)(unaff_A6 + 0xf) < *(short *)((int)unaff_A6 + 0x72))) < '\0') {
      *unaff_A6 = candidate_857de;
    }
    uVar4 = 0;
  }
  else {
    candidate_1abc8();
    candidate_85850();
  }
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_858c4;
  }
  if (*(char *)((int)unaff_A6 + 0x87) != '\0') {
    *(char *)((int)unaff_A6 + 0x87) = *(char *)((int)unaff_A6 + 0x87) + -1;
  }
  return;
}
