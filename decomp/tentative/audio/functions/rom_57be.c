/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x57be; tentative_c. */

byte z80_candidate_57be(ushort *param_1)

{
  char cVar1;
  ushort uVar2;

  uVar2 = *param_1;
  cVar1 = ((char)param_1[1] * '\x02' + CARRY2(uVar2,uVar2)) * '\x02' + CARRY2(uVar2 * 2,uVar2 * 2);
  if (DAT_ram_ff13 != cVar1) {
    DAT_ram_ff13 = cVar1;
  }
  return (byte)(uVar2 >> 8) & 0x3f;
}
