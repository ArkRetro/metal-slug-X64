/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0be770.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_be770(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9f88();
  candidate_9ea8();
  *unaff_A6 = 0xbe78a;
  uVar3 = 0;
  candidate_1abc8();
  candidate_be446();
  if ((bool)uVar3) {
    return;
  }
  candidate_427a();
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) == 0) {
    candidate_409a();
    if ((bool)uVar3) {
      return;
    }
  }
  else {
    candidate_87c0e();
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
