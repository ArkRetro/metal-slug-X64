/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x001004.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1004(void)

{
  ushort uVar1;
  short sVar2;
  byte bVar3;
  undefined2 *puVar4;
  undefined4 *puVar5;
  undefined2 *puVar6;
  undefined4 *puVar7;

  if (_DAT_0000010e < (undefined4 *)0x101800) {
    uVar1 = _DAT_00000112 >> 2;
    puVar5 = _DAT_0000010e;
    puVar7 = (undefined4 *)0x101800;
    do {
      *puVar7 = *puVar5;
      uVar1 = uVar1 - 1;
      puVar5 = puVar5 + 1;
      puVar7 = puVar7 + 1;
    } while (uVar1 != 0xffff);
  }
  sVar2 = 0x8a4;
  puVar4 = (undefined2 *)0x1158;
  puVar6 = (undefined2 *)&SUB_00100000;
  do {
    DAT_00300001 = (undefined1)sVar2;
    *puVar6 = *puVar4;
    sVar2 = sVar2 + -1;
    puVar4 = puVar4 + 1;
    puVar6 = puVar6 + 1;
  } while (sVar2 != -1);
  bVar3 = func_0x00100000();
  if (_DAT_0000010e < (undefined4 *)0x101800) {
    uVar1 = _DAT_00000112 >> 2;
    puVar5 = _DAT_0000010e;
    puVar7 = (undefined4 *)0x101800;
    do {
      *puVar5 = *puVar7;
      uVar1 = uVar1 - 1;
      puVar5 = puVar5 + 1;
      puVar7 = puVar7 + 1;
    } while (uVar1 != 0xffff);
  }
  DAT_0010fd80 = bVar3 & 0x80 | DAT_0010fd80;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}
