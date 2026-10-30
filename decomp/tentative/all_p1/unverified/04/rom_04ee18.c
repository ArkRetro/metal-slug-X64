/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ee18.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ee18(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x6001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x6001;
  candidate_9ea8();
  candidate_4520();
  candidate_4256();
  candidate_1ad6e();
  *unaff_A6 = 0x4ee62;
  uVar3 = 0;
  candidate_1abc8();
  candidate_44f6();
  candidate_427a();
  candidate_4f960();
  if ((bool)uVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_4f94c();
  return;
}
