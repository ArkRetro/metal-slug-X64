/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x083b30.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_83b30(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  candidate_1ad6e();
  candidate_1aca2();
  *unaff_A6 = 0x83b6a;
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  uVar3 = 0;
  candidate_427a();
  candidate_409a();
  if (!(bool)uVar3) {
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
