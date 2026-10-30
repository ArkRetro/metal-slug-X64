/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0539e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_539e0(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  *(undefined2 *)(unaff_A6 + 0x1f) = 0x8e;
  unaff_A6[0x14] = 0x52abe;
  candidate_1ad6e();
  *unaff_A6 = 0x53a0c;
  bVar4 = false;
  candidate_409a();
  if (!bVar4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1abc8();
  candidate_4c24();
  cVar3 = '\0';
  if (*(char *)(unaff_A6[3] + 0x83) == '\0') {
    cVar3 = -(*(char *)((int)unaff_A6 + 0x5a) != '\0');
  }
  *(char *)((int)*(short *)(unaff_A6 + 0x1f) + unaff_A6[3]) = cVar3;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
