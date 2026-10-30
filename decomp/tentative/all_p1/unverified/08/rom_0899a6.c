/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0899a6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_899a6(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_4256();
  candidate_8a528();
  *unaff_A6 = 0x899c0;
  candidate_89a68();
  sVar3 = candidate_8a480();
  if (sVar3 == 0) {
    *unaff_A6 = candidate_899f8;
  }
  if (sVar3 < 0) {
    candidate_8a4f6();
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = *(undefined4 *)((int)unaff_A6 + 0x6e);
  candidate_1456a();
  candidate_427a();
  candidate_8a37c();
  *(undefined1 *)((int)unaff_A6 + 0x89) = 0;
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
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
  candidate_8a654();
  *(char *)((int)unaff_A6 + 0x8e) = -bVar4;
  if (bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 1;
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x66) = *(undefined2 *)(unaff_A6 + 0x20);
    *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6 + 0x20);
  }
  candidate_1ad8c();
  if (*(char *)((int)unaff_A6 + 0x8a) == '\0') {
    candidate_4c24();
  }
  else {
    *(char *)((int)unaff_A6 + 0x8a) = *(char *)((int)unaff_A6 + 0x8a) + -1;
  }
  if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) &&
     (*unaff_A6 = candidate_89e34, *(char *)((int)unaff_A6 + 0x5a) == '\a')) {
    *unaff_A6 = candidate_89ed6;
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if (*(char *)((int)unaff_A6 + 0x85) != '\0') {
    *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + -1;
  }
  if (*(short *)((int)unaff_A6 + 0x82) < *(short *)(unaff_A6 + 10)) {
    *unaff_A6 = candidate_8a00a;
  }
  if (*(char *)(unaff_A6 + 0x23) != *(char *)((int)unaff_A6 + 0x8d)) {
    *(char *)((int)unaff_A6 + 0x8d) = *(char *)(unaff_A6 + 0x23);
    candidate_9d1e();
  }
  return;
}
