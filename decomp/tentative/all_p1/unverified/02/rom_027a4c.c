/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x027a4c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_27a4c(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  byte in_D0b;
  short sVar5;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar6;
  int unaff_A6;
  bool bVar7;

  if (*(char *)((short)((ushort)in_D0b << 3) + 0x27c7b) == '\0') {
    bVar7 = false;
    candidate_40bc();
    iVar6 = extraout_A0;
    if (bVar7) {
      candidate_1b220();
      iVar6 = extraout_A0_00;
    }
  }
  else {
    bVar7 = false;
    candidate_40bc();
    iVar6 = extraout_A0_01;
    if (bVar7) {
      candidate_1b220();
      iVar6 = extraout_A0_02;
    }
  }
  *(byte *)(iVar6 + 0x87) = in_D0b;
  *(undefined1 *)(iVar6 + 0x88) = *(undefined1 *)(unaff_A6 + 0x88);
  sVar5 = (ushort)in_D0b << 3;
  uVar1 = *(undefined2 *)(sVar5 + 0x27c74);
  uVar2 = *(undefined2 *)(sVar5 + 0x27c76);
  uVar3 = *(undefined1 *)(sVar5 + 0x27c78);
  uVar4 = *(undefined1 *)(sVar5 + 0x27c79);
  *(undefined1 *)(iVar6 + 0x8b) = *(undefined1 *)(sVar5 + 0x27c7a);
  *(undefined2 *)(iVar6 + 0x7a) = uVar1;
  *(undefined2 *)(iVar6 + 0x7c) = uVar2;
  *(undefined1 *)(iVar6 + 0x89) = uVar3;
  *(undefined1 *)(iVar6 + 0x8a) = uVar4;
  return;
}
