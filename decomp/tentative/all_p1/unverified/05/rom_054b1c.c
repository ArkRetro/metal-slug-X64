/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x054b1c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_54b1c(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  char cVar4;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_9ea8();
  candidate_1ab98();
  candidate_12022();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_4256();
  *unaff_A6 = 0x54b66;
  if (*(char *)(unaff_A6[3] + 0x91) == '\0') {
    candidate_1abc8();
    candidate_427a();
    return;
  }
  *unaff_A6 = 0x54b86;
  cVar4 = *(char *)((int)unaff_A6 + 0x8a) + -1;
  *(char *)((int)unaff_A6 + 0x8a) = cVar4;
  if (cVar4 != '\0') {
    candidate_1abc8();
    candidate_427a();
    return;
  }
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0x80000;
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0x48;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0x48;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 8;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff1;
  *unaff_A6 = 0x54bf0;
  if (*(char *)((int)unaff_A6 + 0x13) == -1) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  bVar3 = *(byte *)((int)unaff_A6 + 0x12);
  *(byte *)((int)unaff_A6 + 0x12) = bVar3 + 5;
  if (0xfa < bVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
  }
  *(undefined1 *)((int)unaff_A6 + 0x13) = *(undefined1 *)((int)unaff_A6 + 0x12);
  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  candidate_1ac38();
  candidate_427a();
  return;
}
