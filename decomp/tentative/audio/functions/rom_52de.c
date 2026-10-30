/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x52de; tentative_c. */

byte z80_candidate_52de(byte param_1,short param_2)

{
  ushort uVar1;
  ushort uVar2;
  byte bVar3;

  if ((*(byte *)(param_2 + 9) & 0x40) == 0) {
    return param_1;
  }
  bVar3 = *(byte *)(param_2 + 0x10) | *(byte *)(param_2 + 0x11);
  if (bVar3 == 0) {
    return bVar3;
  }
  uVar1 = *(ushort *)CONCAT11(0xc,*(char *)(param_2 + 0x13) * '\x02');
  uVar2 = *(ushort *)(param_2 + 0x10);
  bVar3 = *(byte *)(param_2 + 0x11);
  if ((char)*(byte *)(param_2 + 0x11) < '\0') {
    if (!CARRY2(uVar2,uVar1)) {
      *(ushort *)(param_2 + 0x10) = uVar2 + uVar1 + (ushort)CARRY2(uVar2,uVar1);
      return bVar3;
    }
  }
  else if (uVar1 <= uVar2) {
    *(ushort *)(param_2 + 0x10) = uVar2 - uVar1;
    return bVar3;
  }
  *(undefined1 *)(param_2 + 0x10) = 0;
  *(undefined1 *)(param_2 + 0x11) = 0;
  return 0;
}
