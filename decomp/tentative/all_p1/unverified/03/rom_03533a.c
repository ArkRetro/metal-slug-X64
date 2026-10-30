/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03533a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3533a(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_1ad6e();
  candidate_4256();
  candidate_9ea8();
  *unaff_A6 = 0x3536c;
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -3;
  candidate_427a();
  cVar3 = candidate_aab84();
  bVar4 = false;
  if (-1 < cVar3) {
    candidate_409a();
    if (bVar4) {
      return;
    }
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
