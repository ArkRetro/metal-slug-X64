/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0757aa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_757aa(void)

{
  short sVar1;
  char cVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_767b0();
  uVar3 = *(byte *)((int)unaff_A6 + 0x12) < 0x10;
  if ('\x10' < (char)*(byte *)((int)unaff_A6 + 0x12)) {
    sVar1 = *(short *)(unaff_A6 + 0x21);
    uVar3 = sVar1 == 0;
    sVar1 = sVar1 + -1;
    *(short *)(unaff_A6 + 0x21) = sVar1;
    if (sVar1 == 0) {
      *(undefined2 *)(unaff_A6 + 0x21) = 1;
      *(char *)((int)unaff_A6 + 0x12) = *(char *)((int)unaff_A6 + 0x12) + -1;
      cVar2 = *(char *)((int)unaff_A6 + 0x13);
      uVar3 = cVar2 == '\0';
      *(char *)((int)unaff_A6 + 0x13) = cVar2 + -1;
    }
  }
  candidate_21288();
  if (!(bool)uVar3) {
    *unaff_A6 = 0x758e4;
    uVar3 = 0;
  }
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_75216;
  }
  return;
}
