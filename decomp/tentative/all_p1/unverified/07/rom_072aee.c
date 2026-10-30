/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x072aee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_72aee(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  *(undefined1 *)((int)unaff_A6 + 0x7b) = 0x7f;
  *unaff_A6 = 0x72afa;
  bVar4 = false;
  candidate_409a();
  if (!bVar4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  cVar3 = candidate_aab84();
  if (cVar3 < '\0') {
    *unaff_A6 = candidate_72aca;
    return;
  }
  candidate_1aca2();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(extraout_A0 + 0x1e);
  *(undefined1 *)(unaff_A6 + 0x1e) = *(undefined1 *)(extraout_A0 + 0x78);
  *(undefined1 *)((int)unaff_A6 + 0x79) = *(undefined1 *)(extraout_A0 + 0x79);
  if (*(short *)(extraout_A0 + 0x70) != *(short *)(unaff_A6 + 0x1c)) {
    *(short *)(unaff_A6 + 0x1c) = *(short *)(extraout_A0 + 0x70);
    candidate_9f88();
    candidate_9ea8();
    *(undefined1 *)((int)unaff_A6 + 0x7b) = 0x7f;
  }
  cVar3 = -(*(char *)(unaff_A6[3] + 0x7a) != '\0');
  if (cVar3 != *(char *)((int)unaff_A6 + 0x7b)) {
    *(char *)((int)unaff_A6 + 0x7b) = cVar3;
    if (cVar3 == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
