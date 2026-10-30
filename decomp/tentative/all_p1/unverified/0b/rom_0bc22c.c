/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bc22c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bc22c(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  cVar3 = candidate_19dca();
  if (cVar3 != '\0') {
    candidate_4256();
    *unaff_A6 = 0xbc252;
    cVar3 = candidate_19dca();
    if (cVar3 == '\0') {
      candidate_4256();
      *unaff_A6 = candidate_bc22c;
    }
  }
  candidate_1abc8();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0xf) < -0x30) {
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
