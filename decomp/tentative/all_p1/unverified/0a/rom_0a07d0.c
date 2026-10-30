/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a07d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a07d0(void)

{
  int iVar1;
  char cVar2;
  short sVar3;
  undefined4 uVar4;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0xc);
  cVar2 = *(char *)(iVar1 + 0x89);
  sVar3 = (short)*(char *)(iVar1 + 0x88);
  if ((*(byte *)(iVar1 + 0x15) & 1) != 0) {
    sVar3 = -sVar3;
  }
  *(short *)(unaff_A6 + 0x28) = sVar3;
  *(short *)(unaff_A6 + 0x2c) = (short)cVar2;
  candidate_1abc8();
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  *(undefined1 *)(unaff_A6 + 0x90) = *(undefined1 *)(iVar1 + 0x90);
  if ((*(byte *)(unaff_A6 + 0x90) & 4) == 0) {
    *(short *)(unaff_A6 + 0x1e) = *(short *)(unaff_A6 + 0x1e) + -1;
    uVar4 = 0xa0aea;
  }
  else {
    uVar4 = 0xa0af6;
  }
  cVar2 = *(char *)(iVar1 + 0x8b);
  if (cVar2 < '\0') {
    uVar4 = 0;
  }
  if (cVar2 == '\r') {
    uVar4 = 0;
  }
  if (cVar2 == '\v') {
    uVar4 = 0;
  }
  if (cVar2 == '\f') {
    uVar4 = 0;
  }
  *(undefined4 *)(unaff_A6 + 0x50) = uVar4;
  if (cVar2 != *(char *)(unaff_A6 + 0x8b)) {
    *(char *)(unaff_A6 + 0x8b) = cVar2;
    candidate_a098c();
  }
  return;
}
