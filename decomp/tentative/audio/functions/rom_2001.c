/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2001; tentative_c. */

/* WARNING: Possible PIC construction at 0x5075: Changing call to branch */
/* WARNING: Possible PIC construction at 0x5087: Changing call to branch */
/* WARNING: Possible PIC construction at 0x20b6: Changing call to branch */
/* WARNING: Possible PIC construction at 0x2088: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x20b9) */
/* WARNING: Removing unreachable block (ram,0x508a) */
/* WARNING: Removing unreachable block (ram,0x509b) */
/* WARNING: Removing unreachable block (ram,0x50ce) */
/* WARNING: Removing unreachable block (ram,0x50a8) */
/* WARNING: Removing unreachable block (ram,0x50b6) */
/* WARNING: Removing unreachable block (ram,0x50c5) */
/* WARNING: Removing unreachable block (ram,0x5078) */
/* WARNING: Removing unreachable block (ram,0x208b) */
/* WARNING: Removing unreachable block (ram,0x20a5) */
/* WARNING: Removing unreachable block (ram,0x4fca) */
/* WARNING: Removing unreachable block (ram,0x500c) */
/* WARNING: Removing unreachable block (ram,0x5019) */
/* WARNING: Removing unreachable block (ram,0x5034) */
/* WARNING: Removing unreachable block (ram,0x5039) */
/* WARNING: Removing unreachable block (ram,0x503a) */
/* WARNING: Removing unreachable block (ram,0x504b) */
/* WARNING: Removing unreachable block (ram,0x20a4) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte z80_candidate_2001(byte *param_1)

{
  byte in_F;
  byte bVar1;
  byte bVar2;
  byte bVar3;
  char cVar4;
  short sVar5;
  undefined1 *puVar6;
  undefined2 uStack_2;

  uStack_2 = 0x2004;
  bVar1 = z80_candidate_1ec1();
  if ((in_F & 1) == 0) {
    return bVar1;
  }
  _DAT_ram_f816 = _DAT_ram_f816 + 1;
  if (bVar1 < 0x20) {
    if (bVar1 != 0xf) {
      _DAT_ram_f818 = CONCAT11(bVar1,DAT_ram_f818);
    }
    if (0xf < bVar1) {
      return bVar1;
    }
    bVar2 = *(byte *)((byte)(bVar1 * '\x02') + 0x2027);
                    /* WARNING: Switch is manually overridden */
    switch(bVar1) {
    default:
z80_candidate_2084:
      return bVar2;
    case 2:
      DAT_ram_f815 = 0;
      bVar3 = 0;
SUB_ram_4faf:
      if (bVar3 == 0) {
        _DAT_ram_f806 = 0xffff;
        return bVar3;
      }
      _DAT_ram_f806 = (ushort)*(byte *)CONCAT11(7,bVar3) * 0x10;
      return ~*(byte *)CONCAT11(6,bVar3);
    case 4:
      uStack_2 = 0x2111;
      FUN_ram_4cab();
      break;
    case 5:
      _DAT_ram_fdf4 = 0;
      puVar6 = (undefined1 *)0xfc00;
      cVar4 = '\x0e';
      do {
        puVar6[9] = 0;
        *puVar6 = 0;
        puVar6[1] = 0;
        puVar6[9] = puVar6[9] | 8;
        puVar6 = puVar6 + 0x23;
        cVar4 = cVar4 + -1;
      } while (cVar4 != '\0');
      return 0;
    case 6:
      break;
    case 0xc:
      DAT_ram_f813 = 0xff;
      return 0xff;
    case 0xf:
      param_1 = (byte *)0x24a8;
      goto LAB_ram_506f;
    }
    puVar6 = (undefined1 *)0xfdf6;
    cVar4 = '\b';
    do {
      puVar6[9] = 0;
      puVar6[9] = puVar6[9] | 8;
      puVar6[2] = 0xff;
      puVar6[3] = 0;
      *puVar6 = 0;
      puVar6[1] = 0;
      puVar6 = puVar6 + 0x23;
      cVar4 = cVar4 + -1;
    } while (cVar4 != '\0');
    return 0;
  }
  _DAT_ram_f818 = CONCAT11(DAT_ram_f819,bVar1);
  DAT_ram_f812 = bVar1 - 0x20;
  if (DAT_ram_f819 < 0x10) {
    bVar2 = *(byte *)((byte)(DAT_ram_f819 * '\x02') + 0x2064);
    bVar3 = DAT_ram_f812;
                    /* WARNING: Switch is manually overridden */
    switch(DAT_ram_f819) {
    case 0:
      bVar3 = DAT_ram_f815;
      break;
    default:
      goto z80_candidate_2084;
    case 7:
      DAT_ram_f814 = DAT_ram_f812;
      return DAT_ram_f812;
    case 8:
      DAT_ram_fded = DAT_ram_f814;
      uStack_2 = 0x4f8c;
      DAT_ram_fdec = z80_candidate_4f90(DAT_ram_fdf3,
                                        CONCAT11(*(undefined1 *)
                                                  ((byte)(DAT_ram_f819 * '\x02') + 0x2065),
                                                 bVar1 >> 1));
      return DAT_ram_fdec;
    case 9:
      DAT_ram_f815 = DAT_ram_f812;
      return DAT_ram_f812;
    case 10:
      DAT_ram_f808 = 1;
      break;
    case 0xb:
      DAT_ram_f808 = 0;
    }
    goto SUB_ram_4faf;
  }
  if (DAT_ram_f813 != '\0') {
    DAT_ram_f813 = '\0';
    sVar5 = _DAT_ram_f818;
    goto SUB_ram_5204;
  }
  sVar5 = (ushort)(byte)((DAT_ram_f819 - 0x10 & 0xf) * '\x02') * 2 + 0x88;
  uStack_2 = 0x20ea;
  z80_candidate_57be();
  param_1 = (byte *)((ushort)DAT_ram_f812 * 4 + sVar5);
  bVar1 = *param_1 | param_1[1] | param_1[2];
  if (bVar1 == 0) {
    return bVar1;
  }
  uStack_2 = 0x20ff;
  z80_candidate_57be(bVar1);
LAB_ram_506f:
  sVar5 = *(short *)param_1;
  register0x44 = (BADSPACEBASE *)&uStack_2;
SUB_ram_5204:
  bVar1 = (byte)sVar5 | (byte)((ushort)sVar5 >> 8);
  if (bVar1 == 0) {
    return bVar1;
  }
  *(byte **)((short)register0x44 + -2) = param_1;
  puVar6 = (undefined1 *)0xfdf6;
  bVar1 = 8;
  do {
    if ((*(short *)(puVar6 + 5) == sVar5) && (_DAT_ram_f816 != *(short *)(puVar6 + 7))) {
      puVar6[9] = 8;
      *puVar6 = 0;
      puVar6[1] = 0;
      puVar6[2] = 0xff;
    }
    puVar6 = puVar6 + 0x23;
    bVar1 = bVar1 - 1;
  } while (bVar1 != 0);
  return bVar1;
}
