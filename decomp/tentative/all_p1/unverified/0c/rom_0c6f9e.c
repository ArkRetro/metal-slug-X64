/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c6f9e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_c6f9e(void)

{
  byte bVar1;
  int in_D0;
  int in_D1;
  int iVar2;
  byte bVar3;
  int in_A0;

  iVar2 = (int)(short)((ushort)(in_D0 * 2) >> 1) * (int)*(short *)(in_A0 + 0x26);
  bVar3 = (byte)iVar2;
  bVar1 = *(byte *)(in_A0 + 0x2a);
  *(byte *)(in_A0 + 0x2a) = bVar3 + bVar1;
  *(uint *)(in_A0 + 8) =
       (iVar2 >> 8) + (uint)CARRY1(bVar3,bVar1) +
       (int)(short)((uint)(in_D0 * 2) >> 0x10) * (int)*(short *)(in_A0 + 0x26) * 0x80;
  iVar2 = (int)(short)((ushort)(in_D1 * 2) >> 1) * (int)*(short *)(in_A0 + 0x28);
  bVar3 = (byte)iVar2;
  bVar1 = *(byte *)(in_A0 + 0x2b);
  *(byte *)(in_A0 + 0x2b) = bVar3 + bVar1;
  *(uint *)(in_A0 + 0xc) =
       (iVar2 >> 8) + (uint)CARRY1(bVar3,bVar1) +
       (int)(short)((uint)(in_D1 * 2) >> 0x10) * (int)*(short *)(in_A0 + 0x28) * 0x80;
  candidate_1ac38();
  return CONCAT44(in_D0,in_D1);
}
