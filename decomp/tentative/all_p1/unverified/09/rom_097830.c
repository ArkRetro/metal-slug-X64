/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x097830.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_97830(void)

{
  code *pcVar1;
  short sVar2;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  candidate_9ea8();
  *(undefined4 *)((int)unaff_A6 + 0x72) =
       *(undefined4 *)((short)(*(short *)(unaff_A6[3] + 0x70) * 4) + 0x9649c);
  candidate_966f0();
  *(undefined2 *)(unaff_A6 + 0x1c) = *(undefined2 *)(unaff_A6[3] + 0x70);
  *unaff_A6 = 0x97886;
  candidate_1abc8();
  iVar3 = unaff_A6[3];
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar3 + 0x1e);
  if (*(short *)(iVar3 + 0x76) != *(short *)((int)unaff_A6 + 0x76)) {
    if (*(short *)(iVar3 + 0x70) != *(short *)(unaff_A6 + 0x1c)) {
      candidate_9f88();
      candidate_9ea8();
      iVar3 = unaff_A6[3];
      *(undefined4 *)((int)unaff_A6 + 0x72) =
           *(undefined4 *)((short)(*(short *)(iVar3 + 0x70) * 4) + 0x9649c);
      *(undefined2 *)(unaff_A6 + 0x1c) = *(undefined2 *)(iVar3 + 0x70);
    }
    candidate_966f0();
    sVar2 = *(short *)((int)unaff_A6 + 0x76);
    *(undefined1 *)((int)unaff_A6 + 0x89) = *(undefined1 *)(iVar3 + 0x89);
    if (sVar2 == 0x20) {
      sVar2 = candidate_4256();
    }
    if (sVar2 == 0x24) {
      candidate_4256();
    }
  }
  if (*(short *)((int)unaff_A6 + 0x76) == 8) {
    *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(iVar3 + 0x86);
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(iVar3 + 0x86);
  }
  if (*(char *)(iVar3 + 0x92) == '\0') {
    candidate_427a();
  }
  if (*(short *)((int)unaff_A6 + 0x76) == 0x40) {
    candidate_4126();
    iVar3 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar3 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
