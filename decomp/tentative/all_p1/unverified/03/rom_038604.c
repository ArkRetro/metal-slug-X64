/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x038604.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_38604(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  char cVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  unaff_A6[0xc] = 0xfffd0000;
  candidate_38016();
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -0x20;
  *(undefined2 *)(unaff_A6 + 0x24) = *(undefined2 *)(unaff_A6 + 0xb);
  unaff_A6[0xd] = 0;
  sVar3 = candidate_29f2();
  *(short *)((int)unaff_A6 + 0x92) = sVar3 + 0x23;
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  *unaff_A6 = 0x38642;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    sVar3 = *(short *)((int)unaff_A6 + 0x92) + -1;
    *(short *)((int)unaff_A6 + 0x92) = sVar3;
    if (sVar3 == 0) {
      cVar4 = candidate_29c6();
      if (cVar4 < '\0') {
        candidate_40bc();
      }
    }
    candidate_3853a();
    candidate_37512();
    candidate_427a();
    return;
  }
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x37ab4;
  uVar5 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar5) {
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
