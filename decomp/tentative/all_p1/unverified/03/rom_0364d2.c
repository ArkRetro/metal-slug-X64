/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0364d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_364d2(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  if (*(char *)((int)unaff_A6 + 0x97) != '\0') {
    sVar3 = candidate_8c0c2();
    if (-1 < sVar3) {
      candidate_8ce42();
      candidate_4192();
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
  }
  candidate_427a();
  return;
}
