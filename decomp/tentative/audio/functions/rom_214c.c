/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x214c; tentative_c. */

void z80_candidate_214c(char *param_1,undefined1 *param_2)

{
  char cVar1;
  undefined1 uVar2;
  undefined1 uVar3;

  *param_2 = 0;
  param_2[0xc] = 0;
  param_2[0xd] = 0;
  cVar1 = *param_1;
  uVar2 = 0;
  uVar3 = 0;
  if (cVar1 != '\0') {
    uVar2 = *(undefined1 *)CONCAT11(0xe,cVar1);
    uVar3 = *(undefined1 *)CONCAT11(0xf,cVar1);
  }
  param_2[2] = uVar2;
  param_2[3] = uVar3;
  cVar1 = param_1[1];
  uVar2 = 0;
  uVar3 = 0;
  if (cVar1 != '\0') {
    uVar2 = *(undefined1 *)CONCAT11(0xe,cVar1);
    uVar3 = *(undefined1 *)CONCAT11(0xf,cVar1);
  }
  param_2[4] = uVar2;
  param_2[5] = uVar3;
  param_2[1] = ~param_1[2];
  cVar1 = param_1[3];
  uVar2 = 0;
  uVar3 = 0;
  if (cVar1 != '\0') {
    uVar2 = *(undefined1 *)CONCAT11(0xe,cVar1);
    uVar3 = *(undefined1 *)CONCAT11(0xf,cVar1);
  }
  param_2[6] = uVar2;
  param_2[7] = uVar3;
  cVar1 = param_1[4];
  uVar2 = 0;
  uVar3 = 0;
  if (cVar1 != '\0') {
    uVar2 = *(undefined1 *)CONCAT11(0xe,cVar1);
    uVar3 = *(undefined1 *)CONCAT11(0xf,cVar1);
  }
  param_2[8] = uVar2;
  param_2[9] = uVar3;
  return;
}
