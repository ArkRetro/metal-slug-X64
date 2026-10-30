/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b39d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b39d2(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  func_0x00009cce();
  cVar3 = *(char *)((int)unaff_A6 + 0x6e) + -1;
  if (*(char *)((int)unaff_A6 + 0x6e) == '\0') {
    cVar3 = '\0';
  }
  *(char *)((int)unaff_A6 + 0x6e) = cVar3;
  candidate_1abc8();
  candidate_427a();
  cVar3 = *(byte *)((int)unaff_A6 + 0x12) + 4;
  if (0xfb < *(byte *)((int)unaff_A6 + 0x12)) {
    cVar3 = -1;
  }
  *(char *)((int)unaff_A6 + 0x12) = cVar3;
  if (*(short *)(unaff_A6 + 0xf) < -0xf) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
