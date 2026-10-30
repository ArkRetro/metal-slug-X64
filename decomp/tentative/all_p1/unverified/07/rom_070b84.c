/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x070b84.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_70b84(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x7a) = *(undefined2 *)(unaff_A6[3] + 0x7a);
  *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)(unaff_A6[3] + 0x7c);
  candidate_1ad6e();
  cVar3 = candidate_c05b8();
  if (cVar3 == '\x02') {
    candidate_4256();
  }
  else if (cVar3 == '\x01') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  candidate_9ea8();
  *unaff_A6 = 0x70c34;
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  if (!(bool)uVar4) {
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
