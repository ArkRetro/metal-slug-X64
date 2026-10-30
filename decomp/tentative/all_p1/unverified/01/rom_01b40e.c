/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01b40e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1b40e(void)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint in_D0;
  byte bVar5;
  byte *in_A0;

  uVar2 = (in_D0 % 36000) % 0xe10;
  *in_A0 = (byte)((in_D0 % 36000) / 0xe10) | (byte)((short)(in_D0 / 36000) << 4);
  uVar3 = uVar2 % 600;
  uVar4 = uVar3 % 0x3c;
  in_A0[1] = (byte)(uVar3 / 0x3c) | (byte)((short)(uVar2 / 600) << 4);
  uVar2 = uVar4 % 6;
  uVar1 = (short)uVar2 + 4;
  bVar5 = (byte)uVar1;
  if (9 < uVar1) {
    bVar5 = (char)uVar2 - 6;
  }
  in_A0[2] = bVar5 | (byte)((short)(uVar4 / 6) << 4);
  return;
}
