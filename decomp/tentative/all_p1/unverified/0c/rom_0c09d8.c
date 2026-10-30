/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c09d8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c09d8(void)

{
  ushort uVar1;
  int extraout_A0;
  int extraout_A0_00;
  undefined1 uVar2;

  uVar2 = false;
  uVar1 = func_0x000040ac();
  if (((bool)uVar2) && (uVar2 = uVar1 < *(ushort *)(extraout_A0 + 0x84), !(bool)uVar2)) {
    *(ushort *)(extraout_A0 + 0x84) = uVar1;
    uVar2 = 0;
    *(byte *)(extraout_A0 + 0x5f) = *(byte *)(extraout_A0 + 0x5f) | 0x40;
    *(byte *)(extraout_A0 + 0x9a) = *(byte *)(extraout_A0 + 0x9a) & 0xfb;
  }
  uVar1 = func_0x000040ac();
  if (((bool)uVar2) && (*(ushort *)(extraout_A0_00 + 0x84) <= uVar1)) {
    *(ushort *)(extraout_A0_00 + 0x84) = uVar1;
    *(byte *)(extraout_A0_00 + 0x5f) = *(byte *)(extraout_A0_00 + 0x5f) | 0x40;
    *(byte *)(extraout_A0_00 + 0x9a) = *(byte *)(extraout_A0_00 + 0x9a) & 0xfb;
  }
  return;
}
