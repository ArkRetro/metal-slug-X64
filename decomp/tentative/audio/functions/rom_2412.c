/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2412; tentative_c. */

void z80_candidate_2412(undefined2 param_1,short param_2)

{
  undefined1 uVar1;
  undefined1 uVar3;
  char cVar2;

  uVar3 = (undefined1)((ushort)param_1 >> 8);
  if (*(char *)(param_2 + 0xc) == '\0') {
    z80_candidate_23d1(*(char *)(param_2 + 10) + (char)param_1);
    cVar2 = *(char *)(param_2 + 0xb);
  }
  else {
    cVar2 = (char)param_1 + -0x40 + *(char *)(param_2 + 0xb);
  }
  uVar1 = z80_candidate_23a7(cVar2);
  *(undefined1 *)(param_2 + 4) = uVar1;
  *(undefined1 *)(param_2 + 5) = uVar3;
  *(undefined1 *)(param_2 + 3) = 0;
  *(undefined1 *)(param_2 + 2) = *(undefined1 *)(param_2 + 9);
  return;
}
