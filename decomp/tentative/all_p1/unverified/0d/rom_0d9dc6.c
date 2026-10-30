/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d9dc6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d9dc6(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_da464();
  *(undefined2 *)(unaff_A6 + 0x1c) = 300;
  candidate_4256();
  *unaff_A6 = 0xd9de6;
  candidate_1abc8();
  candidate_da3a8();
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x7b) == -1) {
    *unaff_A6 = candidate_d9e20;
  }
  if (*(char *)(unaff_A6[3] + 0x7c) == -1) {
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
