/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa37c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa37c(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x1c) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0xaa3c8;
  if (*(char *)(unaff_A6[3] + 0x7c) != '\0') {
    *unaff_A6 = candidate_aa3de;
  }
  uVar3 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_44f6();
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
