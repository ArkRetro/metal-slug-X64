/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6de4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a6de4(void)

{
  byte bVar1;
  byte bVar2;
  byte in_D0b;
  int iVar3;
  int extraout_A0;
  int extraout_A0_00;
  int iVar4;
  int unaff_A6;
  bool bVar5;

  bVar5 = false;
  candidate_40bc();
  iVar4 = extraout_A0;
  if (bVar5) {
    candidate_1b220();
    bVar5 = true;
    iVar4 = extraout_A0_00;
  }
  if (bVar5) {
    *(undefined4 *)(iVar4 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
  }
  *(byte *)(iVar4 + 0x87) = in_D0b;
  *(undefined1 *)(iVar4 + 0x7a) = *(undefined1 *)(unaff_A6 + 0x7a);
  iVar3 = (uint)in_D0b * 10;
  *(undefined2 *)(iVar4 + 0x82) = *(undefined2 *)(iVar3 + 0x2a601e);
  *(undefined2 *)(iVar4 + 0x84) = *(undefined2 *)(iVar3 + 0x2a6020);
  *(undefined2 *)(iVar4 + 0x1e) =
       *(undefined2 *)((short)((ushort)*(byte *)(iVar3 + 0x2a6022) * 2) + 0x2a6016);
  bVar1 = *(byte *)(unaff_A6 + 0x15);
  bVar2 = *(byte *)(iVar3 + 0x2a6023);
  *(byte *)(iVar4 + 0x86) = bVar2;
  *(byte *)(iVar4 + 0x15) = *(byte *)(iVar4 + 0x15) & 0xf8;
  *(byte *)(iVar4 + 0x15) = bVar2 & 7 ^ bVar1 & 0xfb | *(byte *)(iVar4 + 0x15);
  if ((*(byte *)(iVar4 + 0x15) & 1) != 0) {
    *(short *)(iVar4 + 0x82) = *(short *)(iVar4 + 0x82) + 1;
  }
  *(undefined4 *)(iVar4 + 0x7e) = *(undefined4 *)(iVar3 + 0x2a601a);
  return;
}
