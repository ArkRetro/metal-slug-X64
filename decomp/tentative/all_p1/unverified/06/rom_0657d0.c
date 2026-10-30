/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0657d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_657d0(void)

{
  undefined1 *puVar1;
  char cVar2;
  int unaff_A5;
  int unaff_A6;

  *(undefined1 *)(unaff_A5 + 0x447b) = *(undefined1 *)(unaff_A5 + 0x447a);
  if (*(char *)(unaff_A6 + 0x74) == -1) {
    return;
  }
  cVar2 = *(char *)(unaff_A6 + 0x74) + -1;
  *(char *)(unaff_A6 + 0x74) = cVar2;
  if (cVar2 == '\0') {
    puVar1 = *(undefined1 **)(unaff_A6 + 0x70);
    *(undefined1 *)(unaff_A6 + 0x74) = *puVar1;
    if (*(char *)(unaff_A6 + 0x74) == -1) {
      *(undefined1 *)(unaff_A6 + 0x75) = 0;
      *(undefined1 *)(unaff_A5 + 0x447a) = 0;
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x75) = puVar1[1];
      *(undefined1 *)(unaff_A5 + 0x447a) = puVar1[2];
      *(int *)(unaff_A6 + 0x70) = *(int *)(unaff_A6 + 0x70) + 4;
    }
  }
  FUN_0000447c[unaff_A5] =
       (code)(*(byte *)(unaff_A5 + 0x447a) &
             (*(byte *)(unaff_A5 + 0x447a) ^ *(byte *)(unaff_A5 + 0x447b)));
  return;
}
